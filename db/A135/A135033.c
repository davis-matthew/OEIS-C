/*
 * A135033  Period 5: repeat [2, 4, 6, 8, 0].
 * https://oeis.org/A135033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*((n+1) mod 5)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pat[5] = { 2, 4, 6, 8, 0 };
    return pat[n % 5];
}
