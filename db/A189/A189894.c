/*
 * A189894  Number of isosceles right triangles on a 2n X (n+1) grid.
 * https://oeis.org/A189894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (22*n^4 + 56*n^3 + 12*n^2 - 24*n - 1 + (-1)^n) / 16
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalently a(n) = -1/16 + 11*n^4/8 + 7*n^3/2 + 3*n^2/4 - 3*n/2 + (-1)^n/16) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the numerator in signed 64‑bit arithmetic.
       For n <= 10000 all intermediate values fit in int64_t. */
    int64_t nn = (int64_t)n;
    int64_t n2 = nn * nn;          /* n^2 */
    int64_t n3 = n2 * nn;          /* n^3 */
    int64_t n4 = n3 * nn;          /* n^4 */

    int64_t term4 = 22 * n4;       /* 22*n^4   */
    int64_t term3 = 56 * n3;       /* 56*n^3   */
    int64_t term2 = 12 * n2;       /* 12*n^2   */
    int64_t term1 = -24 * nn;      /* -24*n    */
    int64_t sign = (n % 2 == 0) ? 1 : -1;  /* (-1)^n */

    int64_t num = -1 + term4 + term3 + term2 + term1 + sign;

    /* The numerator is always a multiple of 16 for integer n >= 1. */
    return (seq_t)(num / 16);
}
