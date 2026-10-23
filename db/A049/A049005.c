/*
 * A049005  Number of letters in English names for days of week.
 * https://oeis.org/A049005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word, dumb, less
 * Formula:   a(n) = a(n-7) for n >= 7; a(0)=6, a(1)=6, a(2)=7, a(3)=9,
 *            a(4)=8, a(5)=6, a(6)=8
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t days[7] = {6, 6, 7, 9, 8, 6, 8};
    return days[n % 7];
}
