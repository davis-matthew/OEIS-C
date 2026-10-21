/*
 * A078209  Smallest multiple of the n-th prime beginning with n.
 * https://oeis.org/A078209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 66, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest multiple of the n-th prime beginning with n
   (OEIS A078209): find the smallest integer M such that
   M is a multiple of the n-th prime p_n and the decimal
   representation of M starts with the decimal digits of n.
   The algorithm searches powers of ten in increasing order:
   for each t, we look for a multiple of p_n in the interval
        [ n·10ᵗ , (n+1)·10ᵗ – 1 ]
   The first such interval that contains a multiple yields the
   minimal M.  The n-th prime is obtained by generating primes
   up to an upper bound n·25+100 (sufficient for n≤10000). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/sieve_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but handle gracefully */

    /* find the n-th prime */
    idx_t limit = n * 25 + 100;                 /* conservative upper bound */
    idx_t *primes = NULL;
    size_t pcnt = 0;

    sieve_primes(limit, &primes, &pcnt);
    if (n > pcnt) {                /* safety check – should not happen */
        free(primes);
        return 0;
    }
    idx_t p = primes[n - 1];
    free(primes);

    seq_t q = (seq_t)p;          /* prime as seq_t for arithmetic */

    seq_t pow10 = 1;              /* 10⁰ */
    while (1) {
        seq_t lower = (seq_t)n * pow10;
        seq_t high  = (seq_t)(n + 1) * pow10 - 1;

        seq_t k = (lower + q - 1) / q;   /* ceil(lower / q) */
        seq_t m = k * q;                /* first multiple >= lower */

        if (m <= high)                 /* found minimal multiple in this range */
            return m;

        /* move to next power of ten; safety guard for overflow */
        pow10 *= 10;
    }
}
