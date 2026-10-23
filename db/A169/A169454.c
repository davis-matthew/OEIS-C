/*
 * A169454  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n + 7*∑_{k=1}^{32} a(n‑k) – 28*a(n‑33) * with p_0 = 1,
 *            p_k = 2 for 1 ≤ k ≤ 32, p_33 = 1, and p_k = 0 otherwise
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Array to hold all computed terms up to n. */
    seq_t *buf = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!buf) return 0;   /* allocation failure – return 0 as a safe fallback */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* Determine the p_n part of the recurrence. */
        seq_t p = 0;
        if (i == 0)
            p = 1;
        else if (i <= 32)
            p = 2;
        else if (i == 33)
            p = 1;   /* only for n = 33; otherwise p = 0 */

        /* Sum of the previous up to 32 terms. */
        seq_t sum = 0;
        idx_t lim = (i < 32 ? i : 32);
        for (idx_t j = 1; j <= lim; ++j)
            sum += buf[i - j];

        /* Term coming from –28*a(n‑33) (zero when i < 33). */
        seq_t term = 0;
        if (i >= 33)
            term = 28 * buf[i - 33];

        /* Compute a(i). */
        buf[i] = p + 7 * sum - term;
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
