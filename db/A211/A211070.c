/*
 * A211070  Number of 2 X 2 matrices having all terms in {1,...,n} and
 *          determinant d satisfying -n < d < n.
 * https://oeis.org/A211070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p=1}^{n^2} f(p) · Σ_{q=max(1,p‑n+1)}^{min(n^2,p+n‑1)}
 *            f(q), where f(p) is the number of ordered pairs (a,d) with
 *            1≤a,d≤n and a·d=p. This implementation pre‑computes f(p) and
 *            then scans the range 1…n² with a sliding window of width 2n‑1 to
 *            evaluate the inner sum in O(n²) time
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* safety, though n≥1 by definition */

    idx_t maxProd = n * n;                     /* largest possible product */
    size_t len    = (size_t)(maxProd + 1);     /* array indices 0 … maxProd */

    /* Allocate and zero‑initialize the frequency array. */
    uint16_t *f = (uint16_t *)calloc(len, sizeof(uint16_t));
    if (!f) return 0;   /* allocation failure – return 0 as safe fallback */

    /* Count occurrences of each product a·d. */
    for (idx_t a_val = 1; a_val <= n; ++a_val) {
        for (idx_t d_val = 1; d_val <= n; ++d_val) {
            idx_t prod = a_val * d_val;
            f[(size_t)prod]++;   /* counts fit in uint16_t up to n=10 000 */
        }
    }

    seq_t ans        = 0;
    seq_t windowSum  = 0;

    idx_t l = 1;      /* left bound of current window (inclusive) */
    idx_t r = 0;      /* right bound of current window (inclusive) */
    idx_t span = n - 1;   /* half‑width of window */

    for (idx_t p = 1; p <= maxProd; ++p) {
        /* Expand the right side of the window to p+span. */
        idx_t newR = p + span;
        if (newR > maxProd) newR = maxProd;
        while (r < newR) {
            ++r;
            windowSum += (seq_t)f[(size_t)r];
        }

        /* Shrink the left side of the window to max(1, p‑span). */
        idx_t newL = (p > span) ? (p - span) : 1;
        while (l < newL) {
            windowSum -= (seq_t)f[(size_t)l];
            ++l;
        }

        /* Add contributions of all matrices with first product p. */
        ans += (seq_t)f[(size_t)p] * windowSum;
    }

    free(f);
    return ans;
}
