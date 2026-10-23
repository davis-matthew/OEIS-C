/*
 * A141811  Partial Catalan numbers: triangle read by rows n = 1, 2, 3, ...
 *          and columns k = 0, 1, ..., n-1.
 * https://oeis.org/A141811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(k) * binom(2r-2k-1 , r-k) * where r is the row index
 *            (r≥1) such that r(r+1)/2 ≥ n, * and k = n - r(r-1)/2 - 1
 *            (0‑based column in that row). * * This implements OEIS A141811
 *            (Partial Catalan numbers), * using the provided helpers
 *            catalan(k) and binom(a,b)
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 153, u64 n <= 595,
 *            u128 n <= 2211, big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/catalan.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find smallest r with r(r+1)/2 ≥ n  (binary search). */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute previous triangular number T(r-1). */
    __int128 prev_tri = (__int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev_tri - 1;   /* 0‑based column */

    seq_t catk = catalan(k);            /* C(k) */
    idx_t v = 2 * r - 2 * k - 1;        /* 2r-2k-1 (≥0) */
    idx_t nk = r - k;                   /* r-k */
    seq_t bin = binom(v, nk);           /* binom(2r-2k-1 , r-k) */

    return catk * bin;
}
