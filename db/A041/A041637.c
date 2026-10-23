/*
 * A041637  Denominators of continued fraction convergents to sqrt(337).
 * https://oeis.org/A041637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1 For n ≥ 1 let c(n) be the continued‑fraction
 *            coefficient of √337, where c(0) = 18 c(n) =
 *            [2,1,3,1,11,2,4,1,3,3,1,4,2,11,1,3,1,2,36][(n‑1) mod 19] Then
 *            a(n) = c(n) * a(n‑1) + a(n‑2) The sequence a(n) is the
 *            denominator of the n‑th convergent to √337. The recurrence uses
 *            only integer arith...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* coefficients of the periodic part of the continued fraction of √337 */
    static const unsigned cf[19] = {
        2, 1, 3, 1, 11, 2, 4, 1, 3, 3,
        1, 4, 2, 11, 1, 3, 1, 2, 36
    };

    /* base values: q_{-1} = 0, q_0 = 1 */
    seq_t q_prev2 = 0;               /* a(-1) */
    seq_t q_prev1 = 1;               /* a(0) */

    if (n == 0)
        return q_prev1;              /* a(0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        /* coefficient c(i) for i ≥ 1 */
        seq_t coeff = (seq_t)cf[(i - 1) % 19];
        /* recurrence: a(i) = c(i) * a(i-1) + a(i-2) */
        seq_t q_i = coeff * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q_i;
    }

    return q_prev1;                  /* a(n) */
}
