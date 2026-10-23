/*
 * A173512  a(n) = 8*n + 4 + n mod 2.
 * https://oeis.org/A173512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n + 4 + n mod 2
 * Range:     u8 n <= 31, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;        /* convert index to seq_t */
    seq_t res = 8 * nn + (seq_t)4;
    res += nn & (seq_t)1;       /* add n mod 2 */
    return res;
}
