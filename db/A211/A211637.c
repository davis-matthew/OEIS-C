/*
 * A211637  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2>x^2+y^2.
 * https://oeis.org/A211637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with 1≤w,x,y≤n and w² >
 *            x² + y²
 * Range:     u8 n <= 10, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Allocate and pre‑compute squares of 1…n */
    seq_t *sq = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!sq) return 0;
    sq[0] = 0;
    for (idx_t i = 1; i <= n; ++i) sq[i] = (seq_t)i * (seq_t)i;

    seq_t total = 0;

    for (idx_t w = 1; w <= n; ++w) {
        /* number of admissible (x,y) for this w */
        seq_t w_sq = sq[w];
        idx_t y = w - 1;                      /* current maximal y with x^2 + y^2 < w^2 */

        for (idx_t x = 1; x <= w - 1; ++x) {
            /* Move y down until squared sum is < w^2 */
            while (y > 0 && sq[x] + sq[y] >= w_sq) {
                --y;
            }
            if (y == 0) break;               /* no further y can satisfy the inequality */
            total += (seq_t)y;                /* all 1 ≤ y' ≤ y are valid */
        }
    }

    free(sq);
    return total;
}
