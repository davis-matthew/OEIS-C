/*
 * A017548  a(n) = (12*n + 2)^4.
 * https://oeis.org/A017548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 2)^4 Recurrence: a(n) = 5*a(n-1) - 10*a(n-2) +
 *            10*a(n-3) - 5*a(n-4) + a(n-5), with a(0)=16, a(1)=38416,
 *            a(2)=456976, a(3)=2085136, a(4)=6250000
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 21, u64 n <= 5461,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)12 * (seq_t)n + (seq_t)2;
    seq_t s = t * t;        /* t² */
    return s * s;           /* (t²)² = t⁴ */
}
