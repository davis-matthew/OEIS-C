/*
 * A194899  Triangular array (and fractal sequence): row n is the permutation
 *          of (1,2,...,n) obtained from the increasing ordering of fractional
 *          parts {r}, {2r}, ..., {nr}, where r=sqrt(12).
 * https://oeis.org/A194899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) term of the triangular array formed by
 *            sorting the numbers 1,…,r by the fractional parts of k*√12,
 *            where r is the unique integer with r(r+1)/2 ≥ n. See OEIS
 *            A194899
 * Range:     u8 n <= 3, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   floor_sqrt, frac_less
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/frac_less.h"

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≥ n. */
    idx_t r = 1;
    while ((seq_t)r * ((seq_t)r + 1) / 2 < n) ++r;

    /* Position of n in this row (1‑based). */
    idx_t prev_tri = (idx_t)(r - 1) * ((idx_t)r) / 2;
    idx_t pos = n - prev_tri;           /* 1‑based position in the row */

    /* Allocate arrays. */
    idx_t *floor_vals = (idx_t *)malloc((r + 1) * sizeof(idx_t));
    idx_t *perm       = (idx_t *)malloc(r * sizeof(idx_t));
    if (!floor_vals || !perm) {
        free(floor_vals);
        free(perm);
        return 0;                       /* allocation failure (unlikely) */
    }

    /* Compute floor(k*√12) and initialize permutation. */
    for (idx_t k = 1; k <= r; ++k) {
        seq_t sq = (seq_t)12 * k * k;
        floor_vals[k] = (idx_t)floor_sqrt(sq);
        perm[k - 1]   = k;
    }

    /* Simple insertion sort using the fractional‑part comparator. */
    for (idx_t i = 1; i < r; ++i) {
        idx_t j = i;
        while (j > 0 && !frac_less(perm[j - 1], perm[j], floor_vals)) {
            idx_t tmp = perm[j];
            perm[j]   = perm[j - 1];
            perm[j - 1] = tmp;
            --j;
        }
    }

    seq_t result = (seq_t)perm[pos - 1];

    free(floor_vals);
    free(perm);
    return result;
}
