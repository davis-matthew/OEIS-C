/*
 * A116916  Expansion of q^(-1/8) * (eta(q)^3 + 3 * eta(q^9)^3) in powers of
 *          q^3.
 * https://oeis.org/A116916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^k * (6*k + 1) if n = k*(3*k + 1)/2 for some integer
 *            k; otherwise a(n) = 0
 * Range:     i8 n <= 2, i16 n <= 621, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute 1 + 24*n and its integer square root. */
    seq_t x = ((seq_t)n) * 24 + 1;
    seq_t s = floor_sqrt(x);

    /* If x is not a perfect square, n cannot be a generalized pentagonal number. */
    if (s * s != x)
        return 0;

    /* Check the positive k solution: n = k*(3k+1)/2  ⇔  sqrt(1+24n) = 6k+1. */
    if ((s - 1) % 6 == 0) {
        seq_t k   = (s - 1) / 6;          /* k ≥ 0 */
        seq_t sign = (k % 2 == 0) ? 1 : -1;
        return sign * (6 * k + 1);
    }

    /* Check the negative k solution: n = k*(3k+1)/2  ⇔  sqrt(1+24n) = 6|k| - 1. */
    if ((s + 1) % 6 == 0) {
        seq_t m   = (s + 1) / 6;          /* m = |k| > 0 */
        seq_t sign = (m % 2 == 0) ? 1 : -1;   /* (-1)^k = (-1)^m */
        /* k = -m ⇒ 6k+1 = -6m+1. */
        return sign * (-(6 * m) + 1);
    }

    /* Should never reach here for a perfect square of the form 1+24n. */
    return 0;
}
