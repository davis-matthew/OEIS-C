/*
 * A066927  Least k such that between p and 2p, for all primes > 3, there is
 *          always a number that is twice a square, i.e.; a k such that p <
 *          2k^2 < 2p.
 * https://oeis.org/A066927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( sqrt( p_n / 2 ) ) + 1
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   _int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* p_n denotes the n‑th prime (p_1 = 2)              */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/_int_sqrt.h"

/* rough upper bound for the n‑th prime (Rosser–Schoenfeld) */
static inline idx_t _prime_limit_for_idx(idx_t n)
{
    if (n < 6) return 15;
    double nd = (double)n;
    double lim = nd * (log(nd) + log(log(nd))) + 3.0;
    return (idx_t)lim;
}

/* return the n‑th prime (n >= 1) */
static idx_t _nth_prime(idx_t n)
{
    idx_t limit = _prime_limit_for_idx(n);
    while (1) {
        /* allocate sieve */
        size_t size = (size_t)(limit + 1);
        uint8_t *sieve = (uint8_t *)calloc(size, sizeof(uint8_t));
        if (!sieve) abort();  /* memory failure */
        for (size_t i = 2; i < size; ++i) sieve[i] = 1;
        for (size_t i = 2; i * i <= (size_t)limit; ++i) {
            if (sieve[i]) {
                for (size_t j = i * i; j <= (size_t)limit; j += i)
                    sieve[j] = 0;
            }
        }
        /* count primes */
        idx_t count = 0;
        idx_t prime = 0;
        for (size_t i = 2; i < size; ++i) {
            if (sieve[i]) {
                ++count;
                if (count == n) { prime = (idx_t)i; break; }
            }
        }
        free(sieve);
        if (count >= n) return prime;
        /* not enough primes, increase limit */
        limit <<= 1;  /* double */
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, safe fallback */

    idx_t p = _nth_prime(n);
    idx_t half = p >> 1;                 /* floor(p/2)           */
    idx_t s = _int_sqrt(half);           /* floor(sqrt(p/2))     */
    return (seq_t)(s + 1);
}
