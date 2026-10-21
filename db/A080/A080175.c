/*
 * A080175  Fourth power of primes of the form 4k+1 (A002144).
 * https://oeis.org/A080175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A002144(n)^4 (fourth power of the n‑th prime ≡1 (mod 4))
 * Range:     u8 none, u16 n <= 2, u32 n <= 24, u64 n <= 3257, u128 n <= 10000,
 *            big any n
 * Helpers:   big_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"   /* next_prime(uint64_t start) */
#include "../utils/big_pow.h"      /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* static cache of primes ≡1 (mod 4) */
    static seq_t *primes4   = NULL;   /* array of found primes          */
    static size_t  primes4_cap = 0;   /* allocated capacity             */
    static size_t  primes4_len = 0;   /* number of stored primes        */
    static idx_t   last_checked = 1;  /* last number examined           */

    if (n == 0) return 0;    /* offset 1, but guard anyway */

    /* Generate further primes until we have at least n of them. */
    while (primes4_len < n) {
        /* obtain next prime > last_checked */
        last_checked = next_prime(last_checked + 1);

        /* keep only primes ≡1 (mod 4) */
        if ((last_checked & 3ULL) == 1ULL) {
            /* grow cache if necessary */
            if (primes4_len + 1 > primes4_cap) {
                size_t new_cap = primes4_cap ? (primes4_cap << 1) : 16;
                seq_t *tmp = (seq_t *)realloc(primes4, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                primes4 = tmp;
                primes4_cap = new_cap;
            }
            primes4[primes4_len++] = last_checked;
        }
    }

    /* Return the fourth power of the requested prime. */
    return big_pow(primes4[n - 1], (seq_t)4);
}
