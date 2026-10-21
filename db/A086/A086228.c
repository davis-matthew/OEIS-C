/*
 * A086228  Determinant of n X n matrix M(i,j) = binomial(2i+1, j).
 * https://oeis.org/A086228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n*(n-3)/2) * (n+1) * binomial(2*n+1, n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the central binomial coefficient */
    seq_t res = binomial(2*n + 1, n);

    /* Multiply by (n+1) */
    res *= ((seq_t)n + 1);

    /* Compute the exponent e = n*(n-3)/2 in a signed 64‑bit integer */
    int64_t e = (int64_t)n * (int64_t)(n - 3) / 2;

    /* Apply the power of two by shifting left or right */
    if (e >= 0)
        res <<= (idx_t)e;
    else
        res >>= (idx_t)(-e);

    return res;
}
