/*
 * A027420  Triangle T00, T10, T01, T20, T11, T02, etc., where Tmn = number of
 *          distinct products ij with min(m,n) <= i,j <= max(m,n).
 * https://oeis.org/A027420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy, nice
 * Formula:   a(n) = number of distinct products i*j with * min(i,j) ≤ i,j ≤
 *            max(i,j), where (i,j) are obtained by * OEIS anti‑diagonal
 *            indexing: (0,0),(1,0),(0,1),(2,0),(1,1),(0,2),…
 * Range:     u8 n <= 405, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

static uint8_t *seen = NULL;
static idx_t seen_cap = 0;

seq_t a(idx_t n)
{
    /* 1. Find diagonal d and offset within it */
    double root = sqrt((double)n * 8.0 + 1.0);
    idx_t d = (idx_t)((root - 1.0) / 2.0);

    /* 2. Correct possible rounding errors */
    while ((d + 1) * (d + 2) / 2 <= n) ++d;
    while (d * (d + 1) / 2 > n) --d;

    idx_t offset = n - d * (d + 1) / 2;
    idx_t i = d - offset;
    idx_t j = offset;

    idx_t l = (i < j) ? i : j;
    idx_t r = (i > j) ? i : j;

    /* 3. Prepare bitset for seen products */
    idx_t max_prod = r * r;          /* r ≤ 140 for n ≤ 10000  */
    if (seen_cap < max_prod + 1) {
        /* reallocate to the needed size */
        free(seen);
        seen = (uint8_t*)calloc((size_t)max_prod + 1, 1);
        seen_cap = max_prod + 1;
    } else {
        memset(seen, 0, (size_t)max_prod + 1);
    }

    /* 4. Count distinct products in [l,r] */
    seq_t count = 0;
    for (idx_t p = l; p <= r; ++p) {
        for (idx_t q = p; q <= r; ++q) {   /* p ≤ q guarantees each product once */
            idx_t prod = p * q;
            if (!seen[prod]) {
                seen[prod] = 1;
                ++count;
            }
        }
    }
    return count;
}
