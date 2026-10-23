/*
 * A115020  Count backwards from 100 in steps of 7.
 * https://oeis.org/A115020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(n) = 100 - 7n
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)100 - ((seq_t)7 * (seq_t)n);
}
