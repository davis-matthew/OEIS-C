/*
 * A139609  a(n) = 36*n + 9.
 * https://oeis.org/A139609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 36*n + 9
 * Range:     u8 n <= 6, u16 n <= 1820, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type before arithmetic */
    seq_t sn = (seq_t)n;
    return sn * 36 + 9;
}
