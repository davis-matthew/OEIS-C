/*
 * A100583  Number of triangles in an n X n grid of squares with diagonals.
 * https://oeis.org/A100583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n^3+18*n^2+4*n+(-1)^n-1)/4
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 1125, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;
    seq_t res = 12 * n3 + 18 * n2 + 4 * ns;

    /* For odd n, (-1)^n - 1 = -2, so subtract 2. */
    if (n & 1)
        res -= 2;

    /* Divide by 4; division is exact. */
    return res / 4;
}
