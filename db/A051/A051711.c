/*
 * A051711  a(0) = 1; for n > 0, a(n) = n!*4^n/2.
 * https://oeis.org/A051711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, frac
 * Formula:   a(n) = n!*4^n/2 (a(0)=1). Recurrence: a(0)=1, a(1)=2,
 *            a(n)=4*n*a(n-1) for n>=2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t res = (seq_t)2;            /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        res *= (seq_t)4 * (seq_t)i;   /* a(i) = 4*i*a(i-1) */
    }

    return res;
}
