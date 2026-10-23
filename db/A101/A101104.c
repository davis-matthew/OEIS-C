/*
 * A101104  a(1)=1, a(2)=12, a(3)=23, and a(n)=24 for n>=4.
 * https://oeis.org/A101104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=12, a(3)=23, a(n)=24 for n>=4
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 12;
    if (n == 3) return 23;
    return 24;
}
