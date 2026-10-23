/*
 * A155158  Period 4: repeat [1, 5, 7, 3].
 * https://oeis.org/A155158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) for n>2; initial values: a(0)=1,
 *            a(1)=5, a(2)=7
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Result is periodic with period 4: [1,5,7,3] */

seq_t a(idx_t n)
{
    static const seq_t period[4] = { (seq_t)1, (seq_t)5, (seq_t)7, (seq_t)3 };
    return period[n & 3];
}
