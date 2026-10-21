/*
 * A208656  Triangle T(n, k) = n*C(n,k) - C(n-1,k-1), 1 <= k <= n, read by
 *          rows.
 * https://oeis.org/A208656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangular entry T(n,k) = n*C(n,k) - C(n-1,k-1); sequence a(n)
 *            is obtained by flattening the triangles rows, starting at offset
 *            1
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer square root: floor(sqrt(x)) for unsigned 64‑bit values. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = x < ((idx_t)1 << 32) ? x : ((idx_t)1 << 32); /* sqrt(ULLONG_MAX)≈4.29e9 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > x / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find row r such that the nth flattened term lies in that row. */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) >> 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Compute k, the column index within row r. */
    idx_t k = n - (r * (r - 1)) / 2;

    /* T(r,k) = r*C(r,k) - C(r-1,k-1) */
    seq_t comb1 = binomial(r, k);      /* C(r,k)   */
    seq_t comb2 = binomial(r - 1, k - 1); /* C(r-1,k-1) */

    return ((seq_t)r) * comb1 - comb2;
}
