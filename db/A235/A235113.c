/*
 * A235113  Irregular triangle read by rows: T(n,k) = number of independent
 *          vertex subsets of size k of the graph g_n obtained by attaching
 *          two pendant edges to each vertex of the complete graph K_n (0 <= k
 *          <= 2n).
 * https://oeis.org/A235113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   binomial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) returns the (n – r²)-th coefficient of
   (1+x)^{2r-2} · (1 + (r+2)x + x²), where r = ⌊√n⌋.
   Equivalently,
   T(r,k) = C(2r-2, k) + (r+2)·C(2r-2, k‑1) + C(2r-2, k‑2),
   with k = n – r² and 0 ≤ k ≤ 2r. */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/mul_small.h"

/* Integer square‑root: floor(√n) for unsigned 64‑bit values. */
static idx_t int_sqrt(idx_t n)
{
    if (n == 0) return 0;
    idx_t low = 1, high = n, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid <= n / mid) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Determine row r: r² ≤ n < (r+1)². */
    idx_t r = int_sqrt(n);
    if (r == 0) return (seq_t)1;      /* the single entry of row 0 */

    /* Position within the row.  k ∈ [0, 2r]. */
    idx_t k = n - r * r;
    idx_t m = 2 * r - 2;               /* exponent in (1+x)^{2r-2} */

    seq_t total = 0;

    if (k <= m)                     /* C(m, k) */
        total += binomial(m, k);

    if (k >= 1 && k - 1 <= m) {     /* (r+2)·C(m, k‑1) */
        seq_t term = mul_small(binomial(m, k - 1), (unsigned int)(r + 2));
        total += term;
    }

    if (k >= 2 && k - 2 <= m)       /* C(m, k‑2) */
        total += binomial(m, k - 2);

    return total;
}
