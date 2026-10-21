/*
 * A200652  a(n) = least k with -3<k<2*n such that n*(n+1)+k and n*(n+1)+k+2
 *          are twin primes, or 0 if no such k exists.
 * https://oeis.org/A200652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least k with -3<k<2*n such that n*(n+1)+k and n*(n+1)+k+2
 *            are twin primes, or 0 if no such k exists
 * Range:     i8 n <= 10, i16 n <= 941, i32 n <= 941, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>          /* for bool type */
#include "../utils/is_prime_big.h"  /* assuming this header defines bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* n is 1‑based; the offset is 1. If n < 1 we return 0 (undefined but harmless). */
    if (n == 0) return 0;            /* guard for accidental 0 index */

    /* Compute n*(n+1) using seq_t arithmetic to avoid overflow. */
    seq_t base = (seq_t)n * ((seq_t)n + 1);

    /* Search for the smallest k in the allowed range. */
    for (seq_t k = -2; k < (seq_t)(2 * n); ++k) {
        seq_t p1 = base + k;
        seq_t p2 = p1 + 2;
        if (is_prime_big(p1) && is_prime_big(p2))
            return k;
    }
    return 0;   /* no suitable k found */
}
