/*
 * A057080  Even-indexed Chebyshev U-polynomials evaluated at sqrt(10)/2.
 * https://oeis.org/A057080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 9, a(n) = 8*a(n-1) - a(n-2) for n >= 2 (derived
 *            from a(-1) = -1, a(0) = 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    seq_t a_prev2 = (seq_t)1;   /* a(0)   */
    seq_t a_prev1 = (seq_t)9;   /* a(1)   */
    seq_t a_cur;

    for (idx_t i = 2; i <= n; ++i) {
        a_cur = (seq_t)8 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_prev1;
}
