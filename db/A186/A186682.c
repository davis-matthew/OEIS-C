/*
 * A186682  Total number of positive integers below 10^n requiring 18 positive
 *          biquadrates in their representation as sum of biquadrates.
 * https://oeis.org/A186682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=2, a(3)=19, a(n)=24 for n>=4
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)19;
    return (seq_t)24;
}
