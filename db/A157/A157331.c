/*
 * A157331  a(n) = 128*n^2 - 32*n + 1.
 * https://oeis.org/A157331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 128*n^2 - 32*n + 1
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 5792, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;         /* cast index to seq_t first */
    seq_t n2 = tn * tn;          /* n^2 */
    seq_t res = n2 * 128;        /* 128 * n^2 */
    res -= tn * 32;              /* subtract 32 * n */
    res += 1;                    /* add 1 */
    return res;
}
