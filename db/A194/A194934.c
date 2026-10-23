/*
 * A194934  Number of lower triangles of a 5 X 5 0..n array with no element
 *          differing from any of its horizontal or vertical neighbors by more
 *          than one.
 * https://oeis.org/A194934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2·a(n-1) – a(n-2) + p_{n-1}, where p_k are the
 *            coefficients of the numerator of the g.f.: P(x) = 32768 +
 *            278555·x + 226904·x² + 60956·x³ + 9052·x⁴ + 1008·x⁵ + 64·x⁶ +
 *            2·x⁷. For k ≥ 8, p_k = 0, which makes the recurrence linear for
 *            n > 6
 * Range:     u8 none, u16 n <= 1, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Coefficients p_k for k = 0..7 */
    static const seq_t p[8] = {
        (seq_t)32768,
        (seq_t)278555,
        (seq_t)226904,
        (seq_t)60956,
        (seq_t)9052,
        (seq_t)1008,
        (seq_t)64,
        (seq_t)2
    };

    if (n == 0)                  /* offset starts at 1, but keep safe */
        return (seq_t)0;
    if (n == 1)
        return p[0];             /* a(1) = 32768 */

    /* a(0) is implicitly 0 for the recurrence to hold */
    seq_t a_prev2 = (seq_t)0;     /* a(0) */
    seq_t a_prev1 = p[0];        /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t pk = (i - 1 < 8) ? p[i - 1] : (seq_t)0;
        a_curr = pk + (a_prev1 << 1) - a_prev2; /* 2·a_prev1 */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
