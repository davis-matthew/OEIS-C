/*
 * A131901  2*A002024 - A131821.
 * https://oeis.org/A131901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r when k = 1 or k = r * a(n) = 2*r – 1 otherwise * where
 *            r is the row index in the triangle, i.e. * r = smallest integer
 *            with r*(r+1)/2 ≥ n * and k = n – (r–1)*r/2 is the column number
 *            (1‑based)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   triangular_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/triangular_value.h"

static inline idx_t triangular_row(idx_t n)
{
    /* binary search: find minimal r such that r*(r+1)/2 >= n */
    idx_t low = 1, high = n; /* r <= n for all n >= 1 */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* undefined for n=0; return 0 to avoid UB */
        return (seq_t)0;
    }

    /* Determine row r */
    idx_t r = triangular_row(n);

    /* Previous triangular number T_{r-1} */
    idx_t prev_tri = triangular_value(r);

    /* Column k (1‑based) */
    idx_t k = n - prev_tri;

    /* Compute result */
    idx_t val;
    if (k == 1 || k == r) {
        val = r;
    } else {
        val = 2 * r - 1;
    }
    return (seq_t)val;
}
