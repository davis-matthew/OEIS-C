/*
 * A170509  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let a(n) be the coefficients of the rational generating function
 *            (t^46 + 2 t^45 + … + 2 t + 1) / (171 t^46 – 18 t^45 – … – 18 t –
 *            1). The sequence satisfies a(0) = 1, a(n) = 2 +
 *            18·∑_{i=1}^{min(n,45)} a(n–i) (1 ≤ n ≤ 45), a(46)= 1 +
 *            18·∑_{i=1}^{45} a(46–i) – 171·a(0), a(n) = 18·∑_{i=1}^{45}
 *            a(n–i...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ----- pre‑compute the first 47 terms *----- */
    static seq_t init[47];
    static bool ready = false;
    if (!ready) {
        for (idx_t i = 0; i <= 46; ++i) {
            seq_t sum = 0;
            idx_t limit = (i < 45 ? i : 45);          /* i‑max 45 terms in sum           */
            for (idx_t j = 1; j <= limit; ++j)
                sum += (seq_t)18 * init[i - j];
            seq_t sub = 0;
            if (i == 46)               /* subtract 171·a(n‑46) only for n = 46   */
                sub = (seq_t)171 * init[0];
            seq_t p = (i == 0 || i == 46) ? (seq_t)1 : (seq_t)2;
            init[i] = p + sum - sub;
        }
        ready = true;
    }

    /* ----- return a(n) for n ≤ 46 *----- */
    if (n <= 46)
        return init[(idx_t)n];

    /* ----- extend the sequence using the 46‑term recurrence *----- */
    seq_t ring[46];                     /* ring[i] = a(cur‑46 + i)           */
    ring[0] = init[1];
    for (idx_t i = 1; i < 46; ++i)
        ring[i] = init[i + 1];         /* ring[45] = a(46) → a(cur‑1) for cur = 47 */

    for (idx_t cur = 47; cur <= n; ++cur) {
        seq_t sum45 = 0;
        for (idx_t i = 1; i < 46; ++i)
            sum45 += ring[i];           /* sum of the last 45 terms           */

        seq_t next = (seq_t)18 * sum45 - (seq_t)171 * ring[0];

        /* shift the ring left and insert the new value */
        for (idx_t i = 0; i < 45; ++i)
            ring[i] = ring[i + 1];
        ring[45] = next;                /* ring[45] becomes a(cur)            */
    }

    return ring[45];                     /* a(n) is stored after the last shift */
}
