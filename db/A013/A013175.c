/*
 * A013175  sin(arctanh(x)+arctan(x))=2*x-8/3!*x^3+80/5!*x^5-4160/7!*x^7...
 * https://oeis.org/A013175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient a(n) of sin(arctanh(x)+arctan(x))
 *            sin(arctanh(x)+arctan(x)) = Σₙ≥0 a(n) · x^(2n+1)/(2n+1)!. The
 *            coefficients are obtained by solving the differential equations
 *            s' = c·f', c' = –s·f' where f' = 2·Σ_{m≥0} x^{4m}. Using the
 *            factorial‑scaled series T_k = s_k·k! (T_k = a(k)) and U_k...
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* maximum degree needed in the ordinary power series */
    idx_t max_d = 2 * n + 1;

    /* running sums for each residue r = 0..3:
       sumS[r] = Σ_{i≡r (mod4)} T_i · (d! / i!),
       sumU[r] = Σ_{i≡r (mod4)} U_i · (d! / i!)  (for current d)   */
    seq_t sumS[4] = {0, 0, 0, 0};
    seq_t sumU[4] = {0, 0, 0, 0};

    /* base case: s0 = 0  (T0 = 0),  c0 = 1  (U0 = 1) */
    sumU[0] = (seq_t)1;    /* U0 contributes to residue 0 */

    seq_t S_next, U_next;
    for (idx_t d = 0; d < max_d; ++d) {
        idx_t r = d & 3u;          /* residue of d modulo 4 */

        /* New values in the factorial‑scaled series */
        S_next = (seq_t)2 * sumU[r];
        U_next = -(seq_t)2 * sumS[r];

        /* If we have reached the required degree, return the answer */
        if (d + 1 == max_d) {
            return S_next;        /* a(n) = T_{2n+1} = S_next */
        }

        /* Prepare sums for the next index (d+1) :
           multiply all by (d+1) and add the new term. */
        seq_t mult = (seq_t)(d + 1);
        for (int rep = 0; rep < 4; ++rep) {
            sumS[rep] = sumS[rep] * mult;
            sumU[rep] = sumU[rep] * mult;
        }
        /* New index (d+1) has residue (d+1) mod 4 */
        sumU[(d + 1) & 3u] = sumU[(d + 1) & 3u] + U_next;
        sumS[(d + 1) & 3u] = sumS[(d + 1) & 3u] + S_next;
    }

    /* control should never reach here */
    return (seq_t)0;
}
