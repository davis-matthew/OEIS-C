/*
 * A010687  Repeat (1,6): Period 2.
 * https://oeis.org/A010687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 6^n mod 7
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 6^n mod 7 alternates: 1 for even n, 6 for odd n */
    return (n & 1ULL) ? 6 : 1;
}
