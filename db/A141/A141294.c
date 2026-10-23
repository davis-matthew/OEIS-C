/*
 * A141294  Triangle read by rows, A054523 * A000012, 1<=k<=n.
 * https://oeis.org/A141294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{d | n, d ≥ k} φ(n/d) where k = n - (r-1)r/2 and r =
 *            ⌈(√(8n+1)-1)/2⌉ (smallest r with r(r+1)/2 ≥ n)
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation for OEIS A141294 */
#include <stdint.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≥ n */
    idx_t r = 0;
    while ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;

    /* Position k in the row (1‑indexed) */
    idx_t before = r * (r - 1) / 2;
    idx_t k = n - before;

    seq_t total = 0;

    /* Iterate over all divisors d of r */
    for (idx_t d = 1; d * d <= r; ++d) {
        if (r % d != 0)
            continue;
        idx_t d1 = d;          /* divisor d1 */
        idx_t d2 = r / d;      /* paired divisor d2 */

        /* Add φ(r/d1) if d1 ≥ k */
        if (d1 >= k)
            total += phi(r / d1);   /* r/d1 == d2 */

        /* Add φ(r/d2) if the pair is distinct and d2 ≥ k */
        if (d2 != d1 && d2 >= k)
            total += phi(r / d2);   /* r/d2 == d1 */
    }

    return total;
}
