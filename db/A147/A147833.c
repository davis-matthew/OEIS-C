/*
 * A147833  Expansion of (4*x^2+x-1)/(12*x^3+4*x^2+x-1).
 * https://oeis.org/A147833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+4*a(n-2)+12*a(n-3), a(0)=1, a(1)=0, a(2)=0
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)0;   /* a(1) */
    seq_t a2 = (seq_t)0;   /* a(2) */
    seq_t ak;

    for (idx_t k = 3; k <= n; ++k) {
        /* ak = a2 + 4*a1 + 12*a0 */
        ak = a2 + (seq_t)4 * a1 + (seq_t)12 * a0;
        /* shift values for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = ak;
    }
    return a2;
}
