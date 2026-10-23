/*
 * A169165  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1
 *          for 1 ≤ n ≤ 26:
 *              a(n) = 2 + 6 * Σ_{k=0}^{n‑1} a(k)
 *          for n ≥ 27:
 *              a(n) = 6 * Σ_{k=n‑26}^{n‑1} a(k) – 21 * a(n‑27)
 *
 * The implementation keeps a circular buffer of the last 27 values
 * and two running sums, so each call runs in O(n) time with only
 * a constant‑size array.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0) handled separately */
    if (n == 0U) {
        return (seq_t)1;
    }

    /* buffer[0] … buffer[26] holds the last 27 terms */
    seq_t buffer[27];
    for (size_t j = 0; j < 27U; ++j) {
        buffer[j] = (seq_t)0;
    }
    buffer[0] = (seq_t)1;                 /* a(0) */

    seq_t sum_all  = (seq_t)1;            /* Σ a(0)…a(i‑1)  */
    seq_t sum_win  = (seq_t)0;            /* Σ of last 26 terms */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t ai;
        if (i <= 26U) {
            ai = (seq_t)2 + (seq_t)6 * sum_all;
            sum_all += ai;
            sum_win += ai;               /* includes a(i) for later use      */
        } else {
            size_t idx       = (size_t)(i % 27U);
            seq_t a_minus_27 = buffer[idx];                           /* a(i‑27) */
            seq_t a_minus_26 = buffer[(size_t)((i - 26U) % 27U)];     /* a(i‑26) */
            ai = (seq_t)6 * sum_win - (seq_t)21 * a_minus_27;

            sum_win += ai;
            sum_win -= a_minus_26;      /* remove a(i‑26) from window for next step */
        }
        buffer[(size_t)(i % 27U)] = ai;
    }

    return buffer[(size_t)(n % 27U)];
}
