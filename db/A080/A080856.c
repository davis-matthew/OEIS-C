/*
 * A080856  a(n) = 8*n^2 - 4*n + 1.
 * https://oeis.org/A080856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 - 4*n + 1
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = 8;     /* start with 8 */
    res *= nn;          /* 8*n */
    res *= nn;          /* 8*n^2 */
    res -= 4 * nn;      /* 8*n^2 - 4*n */
    res += 1;           /* +1 */
    return res;
}
