/*
 * A075894  Average of four successive primes squared, (prime(n)^2 +
 *          prime(n+1)^2 + prime(n+2)^2 + prime(n+3)^2)/4, n>=2.
 * https://oeis.org/A075894
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (prime(n)^2 + prime(n+1)^2 + prime(n+2)^2 +
 *            prime(n+3)^2)/4, n>=2
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* Keep a static list of primes across calls */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;

    /* Number of primes needed for this n (prime(n+3) is the last one) */
    idx_t need = n + 3;

    /* If we do not yet have enough primes, generate a larger sieve */
    while (primes_cnt < (size_t)need)
    {
        idx_t limit;

        /* First call: pick a reasonable starting limit; afterwards double */
        if (primes_limit == 0) {
            limit = 20 * need;            /* works for n up to many thousands */
        } else {
            limit = primes_limit * 2;
        }

        uint64_t *new_primes = NULL;
        size_t new_cnt = 0;
        build_primes_upto(limit, &new_primes, &new_cnt);

        /* Replace old array */
        if (primes) free(primes);
        primes = new_primes;
        primes_cnt = new_cnt;
        primes_limit = limit;
    }

    /* Retrieve the four consecutive primes (prime(1) == primes[0]) */
    seq_t p1 = (seq_t)primes[n - 1];
    seq_t p2 = (seq_t)primes[n];
    seq_t p3 = (seq_t)primes[n + 1];
    seq_t p4 = (seq_t)primes[n + 2];

    /* Compute the average of their squares */
    seq_t sum = p1 * p1 + p2 * p2 + p3 * p3 + p4 * p4;
    return sum / 4;
}
