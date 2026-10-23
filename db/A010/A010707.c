/*
 * A010707  Period 2: repeat (3,9).
 * https://oeis.org/A010707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -3*(-1)^n + 6
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0)=3, a(1)=9, a(2)=3, a(3)=9, ...  */
    return ((n & 1ULL) == 0) ? (seq_t)3 : (seq_t)9;
}
