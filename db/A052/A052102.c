/*
 * A052102  The second of the three sequences associated with the polynomial
 *          x^3 - 2.
 * https://oeis.org/A052102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3), with a(0) = 0, a(1) = 1,
 *            a(2) = 2
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 56, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the base cases */
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;

    /* iterative recurrence */
    seq_t a0 = (seq_t)0;                /* a(i-3)   */
    seq_t a1 = (seq_t)1;                /* a(i-2)   */
    seq_t a2 = (seq_t)2;                /* a(i-1)   */
    seq_t ai;

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        ai = (seq_t)3 * a2 - (seq_t)3 * a1 + (seq_t)3 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return a2;
}
