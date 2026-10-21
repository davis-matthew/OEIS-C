/*
 * A054523  Triangle read by rows: T(n,k) = phi(n/k) if k divides n, T(n,k)=0
 *          otherwise (n >= 1, 1 <= k <= n).
 * https://oeis.org/A054523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = φ(r/k) if r mod k == 0 else 0, * where r is the row index
 *            such that r(r+1)/2 ≥ n, * and k = n – (r−1)r/2 (1 ≤ k ≤ r)
 * Range:     u8 n <= 7, u16 n <= 1313, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   floor_sqrt, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence is 1‑based; return 0 for n=0 */
        return 0;

    /* Determine row r satisfying r(r+1)/2 ≥ n */
    seq_t d   = floor_sqrt((seq_t)8 * (seq_t)n + 1);
    idx_t r   = (idx_t)((d - 1) / 2);            /* preliminary r */

    seq_t tri = (seq_t)r * (r + 1) / 2;
    if (tri < (seq_t)n)                         /* correct if needed */
        ++r;

    /* Column k within the r‑th row (1‑based) */
    seq_t tri_prev = (seq_t)r * (r - 1) / 2;
    idx_t k        = (idx_t)((seq_t)n - tri_prev);

    /* If r is not divisible by k, the value is 0 */
    if (r % k != 0)
        return 0;

    /* Otherwise return φ(r/k) */
    idx_t m = r / k;
    return phi(m);
}
