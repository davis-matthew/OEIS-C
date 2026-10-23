/*
 * A164267  A Fibonacci convolution.
 * https://oeis.org/A164267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 1, a(2) = 2, a(3) = 7 a(n) = 2*a(n-1) +
 *            3*a(n-2) – 4*a(n-3) + a(n-4) for n ≥ 4 The implementation
 *            recomputes the recurrence up to n each time; it uses only
 *            integer arithmetic allowed by the requirements
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)7;

    seq_t a0 = (seq_t)0;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)2;   /* a(2) */
    seq_t a3 = (seq_t)7;   /* a(3) */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i)
    {
        an = (seq_t)2 * a3 + (seq_t)3 * a2 - (seq_t)4 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
