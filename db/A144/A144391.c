/*
 * A144391  a(n) = 3*n^2 + n - 1.
 * https://oeis.org/A144391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 + n - 1
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result = 3;
    result *= nn; /* 3*n */
    result *= nn; /* 3*n^2 */
    result += nn; /* 3*n^2 + n */
    result -= 1;  /* 3*n^2 + n - 1 */
    return result;
}
