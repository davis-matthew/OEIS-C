/*
 * A108476  Expansion of (1-4*x)/(1-6*x-12*x^2+8*x^3).
 * https://oeis.org/A108476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1)+12*a(n-2)-8*a(n-3) with initial values a(0)=1,
 *            a(1)=2, a(2)=24
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)24;

    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)2;   /* a(n-2)   */
    seq_t a2 = (seq_t)24;  /* a(n-1)   */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)6 * a2 + (seq_t)12 * a1 - (seq_t)8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
