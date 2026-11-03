/*
 * A027422  Triangle T00, T10, T11, T20, T21, T22, etc. where Tmn = number of
 *          distinct products ij with m-n<=i,j<=m.
 * https://oeis.org/A027422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 431, u16 n <= 1890, u32 n <= 1890, u64 n <= 1890,
 *            u128 n <= 1890, big any n
 * Validated: standard, pass, 97 terms through n = 1851 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the number of distinct products i·j with m‑k ≤ i,j ≤ m
   where (m,k) are obtained from the flat index n via the triangle
   T_{m,k}.  The mapping is defined by
      n = (m+1)(m+2)/2 + k   with 0 ≤ k ≤ m. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t distinct_product_count(idx_t m, idx_t k)
{
    idx_t L = m - k;                            /* start of the range */
    size_t maxprod = (size_t)m * (size_t)m;     /* maximum possible product */

    /* Allocate a boolean array to mark seen products. */
    bool *seen = (bool*)calloc(maxprod + 1, sizeof(bool));
    if (!seen) return 0;                       /* OOM fallback */

    seq_t cnt = 0;
    for (idx_t i = L; i <= m; ++i) {
        for (idx_t j = L; j <= m; ++j) {
            size_t prod = (size_t)i * (size_t)j;
            if (!seen[prod]) {
                seen[prod] = true;
                ++cnt;
            }
        }
    }
    free(seen);
    return cnt;
}

seq_t a(idx_t n)
{
    /* Find row m such that (m+1)(m+2)/2 > n. */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n) ++m;

    /* compute k within the row */
    idx_t prev_tri = m * (m + 1) / 2;  /* number of terms before row m */
    idx_t k = n - prev_tri;

    return distinct_product_count(m, k);
}
