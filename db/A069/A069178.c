/*
 * A069178  Centered 21-gonal numbers.
 * https://oeis.org/A069178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (21*n^2 - 21*n + 2)/2
 * Range:     u8 n <= 3, u16 n <= 78, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset is 1, undefined for 0 */
    seq_t nn = (seq_t)n;
    seq_t prod = (seq_t)21 * nn * nn;    /* 21 * n^2 */
    seq_t sub  = (seq_t)21 * nn;        /* 21 * n   */
    seq_t res  = (prod - sub + (seq_t)2) / (seq_t)2;
    return res;
}
