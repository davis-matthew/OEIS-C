/*
 * A041910  Numerators of continued fraction convergents to sqrt(477).
 * https://oeis.org/A041910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of convergent k to sqrt(477); recurrence:
 *            p_{-2}=0, p_{-1}=1; for k≥0: p_k = a_k * p_{k-1} + p_{k-2}; a_0
 *            = 21, a_k for k≥1 follows periodic sequence of length 20:
 *            [1,5,3,1,4,10,1,2,2,4,2,2,1,10,4,1,3,5,1,42]. a(n) = p_n
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t cf[20] = {
        1,5,3,1,4,10,1,2,2,4,
        2,2,1,10,4,1,3,5,1,42
    };
    const idx_t period = 20;

    seq_t prev1 = (seq_t)1;   /* p_{-1} */
    seq_t prev2 = (seq_t)0;   /* p_{-2} */
    seq_t cur;
    idx_t k;

    for (k = 0; k <= n; ++k) {
        seq_t coeff = (k == 0) ? (seq_t)21
                               : (seq_t)cf[(k - 1) % period];
        cur = coeff * prev1 + prev2;
        if (k == n)
            return cur;
        prev2 = prev1;
        prev1 = cur;
    }
    /* Unreachable, but keeps the compiler happy. */
    return (seq_t)0;
}
