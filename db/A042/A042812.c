/*
 * A042812  Numerators of continued fraction convergents to sqrt(937).
 * https://oeis.org/A042812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of √937 (n = 0 gives the
 *            first numerator 30)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(sqrt(937)) */
    const seq_t a0 = (seq_t)30U;
    if (n == 0)
        return a0;

    /* p_{-1} and p_0 in standard notation */
    seq_t prev2 = (seq_t)1U;   /* p_{-1} */
    seq_t prev1 = a0;          /* p_0   */

    /* Parameters of the continued fraction for √937 */
    unsigned long long m = 0ULL;
    unsigned long long d = 1ULL;
    unsigned long long a_cur = (unsigned long long)a0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update continued‑fraction parameters */
        m = d * a_cur - m;
        unsigned long long tmp = (unsigned long long)937ULL - m * m;
        d = tmp / d;
        a_cur = (unsigned long long)(a0 + (seq_t)m) / d;

        /* New numerator p_k = a_k * p_{k-1} + p_{k-2}  */
        seq_t curr = (seq_t)a_cur * prev1 + prev2;

        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
