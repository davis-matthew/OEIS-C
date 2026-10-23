/*
 * A144829  Partial products of successive terms of A017209; a(0)=1 .
 * https://oeis.org/A144829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n - 5) * a(n-1) with a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;            /* a(0) = 1 */
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t factor = (seq_t)(9 * i - 5);  /* 9*i-5 fits in seq_t for all practical n */
        res *= factor;
    }
    return res;
}
