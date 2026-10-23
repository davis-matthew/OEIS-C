/*
 * A193678  Discriminant of Chebyshev C-polynomials.
 * https://oeis.org/A193678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n-1)) * n^n, n >= 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* optional, not used in OEIS offset 1 */

    seq_t result = (seq_t)1;
    /* compute 2^(n-1) */
    for (idx_t i = 1; i < n; ++i) {
        result *= (seq_t)2;
    }
    /* compute n^n */
    seq_t nn = (seq_t)n;
    for (idx_t i = 1; i <= n; ++i) {
        result *= nn;
    }
    return result;
}
