/*
 * A010142  Continued fraction for sqrt(57).
 * https://oeis.org/A010142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(0)=7; a(n)=period[(n-1)%6] for n>0, period={1,1,4,1,1,14}
 * Range:     u8 n <= 9878, u16 n <= 9878, u32 n <= 9878, u64 n <= 9878,
 *            u128 n <= 9878, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 7;
    static const seq_t period[6] = {1, 1, 4, 1, 1, 14};
    return period[(n - 1) % 6];
}
