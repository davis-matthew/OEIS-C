/*
 * A127392  Expansion of the elliptic function sqrt(k(q))/q^(1/4) in powers of
 *          q, where sqrt(k(q)) = theta_2(q)/theta_3(q).
 * https://oeis.org/A127392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of * 2 * ∏_{m≥1} (1 - q^m)^2 (1 -
 *            q^{4m})^4 / (1 - q^{2m})^6 * in the power series in q.
 *            Equivalently, it is the Euler transform * of the period‑4
 *            sequence [-2, 4, -2, 0], multiplied by 2
 * Range:     i8 n <= 4, i16 n <= 14, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) return (seq_t)2;

    idx_t N = n;                     /* local copy for array sizes      */
    seq_t *s = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *g = (seq_t*)malloc((size_t)(N + 1) * sizeof(seq_t));

    /* Compute s[m] = ∑_{d|m} d * a_d, where a_d follows the period‑4 pattern:
     *   d≡0 (mod 4) → a_d = 0
     *   d≡2 (mod 4) → a_d = -4
     *   otherwise   → a_d =  2
     */
    for (idx_t d = 1; d <= N; ++d) {
        seq_t a_d;
        switch (d & 3) {
            case 0: a_d = 0;            break;
            case 2: a_d = -4;           break;
            default: a_d = 2;           break;
        }
        seq_t contrib = a_d * (seq_t)d;
        for (idx_t m = d; m <= N; m += d) {
            s[m] += contrib;
        }
    }

    /* Recurrence: g[0] = 1, and for n ≥ 1
     *   g[n] = -(1/n) * Σ_{k=1}^{n} s[k] * g[n-k]
     * The result a(n) = 2 * g[n].
     */
    g[0] = 1;
    for (idx_t nn = 1; nn <= N; ++nn) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= nn; ++k) {
            sum += s[k] * g[nn - k];
        }
        g[nn] = - (sum / (seq_t)nn);
    }

    seq_t result = g[N] * 2;

    free(s);
    free(g);
    return result;
}
