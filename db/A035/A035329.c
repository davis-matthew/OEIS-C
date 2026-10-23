/*
 * A035329  a(n) = n*(2*n+5)*(2*n+7).
 * https://oeis.org/A035329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n+5)*(2*n+7)
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t t1 = (sn << 1) + (seq_t)5;  /* 2*n + 5 */
    seq_t t2 = (sn << 1) + (seq_t)7;  /* 2*n + 7 */
    return sn * t1 * t2;
}
