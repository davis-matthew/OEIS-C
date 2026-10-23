/*
 * A180357  a(n) = n^7 + 7*n.
 * https://oeis.org/A180357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^7 + 7*n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                    /* cast index to sequence type */
    seq_t pow = sn * sn * sn * sn * sn * sn * sn;  /* n^7 */
    return pow + (seq_t)7 * sn;              /* n^7 + 7*n */
}
