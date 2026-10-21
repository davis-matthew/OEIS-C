/*
 * A090585  Numerator of (Sum_{k=1..n} k) / (Product_{k=1..n} k).
 * https://oeis.org/A090585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = gcd( n*(n+1)/2 , n! + 1 ) * That is the numerator of
 *            (1+2+...+n)/(1*2*...*n), reduced to lowest terms. * The
 *            implementation computes n! modulo A = n(n+1)/2, then uses the
 *            Euclidean * algorithm via the provided gcd routine
 * Range:     u8 n <= 27, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute A = n*(n+1)/2 as seq_t */
    seq_t A = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Compute n! modulo A */
    seq_t r = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        r = (r * (seq_t)i) % A;
    }

    /* Compute (n! + 1) mod A */
    seq_t m = (r + 1) % A;

    /* The numerator is gcd(A, m) */
    return gcd(A, m);
}
