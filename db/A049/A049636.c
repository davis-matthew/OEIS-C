/*
 * A049636  Congruent to 0 or 2 mod 3, but not equal to 0 or 3.
 * https://oeis.org/A049636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(n)=3+2*n - floor(n/2) for n>0
 * Range:     u8 n <= 168, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)2;
    }
    seq_t sn  = (seq_t)n;
    seq_t half = sn >> 1;     /* floor(n/2) */
    return (seq_t)3 + sn * 2 - half;
}
