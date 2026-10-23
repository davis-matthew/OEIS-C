/*
 * A041792  Numerators of continued fraction convergents to sqrt(417).
 * https://oeis.org/A041792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   Numerators of continued‑fraction convergents to sqrt(417)
 *            Recurrence: p_{-1}=1, p_0=20, p_n = a_n * p_{n-1} + p_{n-2},
 *            where a_n repeats every 18:
 *            2,2,1,1,1,5,4,1,12,1,4,5,1,1,1,2,2,40. The function returns
 *            a(n)=p_n for n≥0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)20;
    }

    /* Periodic partial quotients for sqrt(417). */
    static const int coeffs[18] = {
        2, 2, 1, 1, 1, 5, 4, 1,
        12, 1, 4, 5, 1, 1, 1, 2,
        2, 40
    };

    seq_t prev2 = (seq_t)1;      /* p_{-1} */
    seq_t prev1 = (seq_t)20;     /* p_0 */
    seq_t curr;

    for (idx_t i = 1; i <= n; ++i) {
        int c = coeffs[(i - 1) % 18];
        curr = (seq_t)c * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
