/*
 * A137780  a(n) = 1 + 2^( prime(n + 1) - prime(n) ).
 * https://oeis.org/A137780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2^(prime(n+1) - prime(n)) (offset 1)
 * Range:     u8 n <= 23, u16 n <= 98, u32 n <= 216, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* offset 1 – a(1) is the first term; undefined for n<1 */
    if (n < 1) return 0;

    /* --- prime cache -------------------------------------------------- */
    static idx_t *primes = NULL;      /* array of primes (prime(1)=2) */
    static size_t primes_cnt = 0;     /* number of primes stored      */
    static size_t primes_cap = 0;     /* allocated capacity           */
    static idx_t last_checked = 1;    /* last integer tested */

    /* need primes[0 … n] → prime(n+1) and prime(n) */
    while (primes_cnt <= n) {
        /* seek next prime */
        ++last_checked;
        if (is_prime(last_checked)) {
            /* grow storage if necessary */
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap << 1 : 16;
                idx_t *tmp = (idx_t *)realloc(primes,
                                              new_cap * sizeof(idx_t));
                if (!tmp) {                /* allocation failure  */
                    free(primes);
                    primes = NULL;
                    primes_cnt = primes_cap = 0;
                    return 0;
                }
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = last_checked;
        }
    }

    /* compute the prime gap and the term */
    idx_t gap = primes[n] - primes[n - 1];
    seq_t pow2 = seq_pow((seq_t)2, gap);
    return pow2 + (seq_t)1;
}
