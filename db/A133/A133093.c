/*
 * A133093  A007318 * A097806 * A133080.
 * https://oeis.org/A133093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   the sequence a(n) is the n-th entry (in row‑major order) of the
 *            * infinite lower‑triangular matrix obtained by binomial
 *            transforming the * matrix with main diagonal 1's, subdiagonal
 *            pattern (2,1,2,1,…), * and subsubdiagonal pattern (1,0,1,0,…).
 *            This matrix is exactly * the product A007318 * A0978...
 * Range:     u8 n <= 38, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom, tri_geq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/tri_geq.h"

seq_t a(idx_t n)
{
    /* --- find the row r such that r(r+1)/2 ≥ n > (r-1)r/2 --- */
    idx_t low = 0, high = n;               /* upper bound is safe */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (tri_geq(mid, n))
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                          /* row index (1‑based) */

    /* --- column index j in the row --- */
    idx_t t_prev = (idx_t)((__int128)(r - 1) * r / 2); /* (r-1)r/2 */
    idx_t j = n - t_prev;                      /* 1‑based column */

    /* --- compute the entry M[r][j] --- */
    seq_t sum = 0;
    idx_t diff_limit = r - j;
    idx_t maxk = (diff_limit >= 2) ? (j + 2) : r;   /* last k to process */

    for (idx_t k = j; k <= maxk; ++k) {
        int diff = (int)(k - j);
        int val = 0;
        if (diff == 0)
            val = 1;
        else if (diff == 1)
            val = ((k & 1) == 0) ? 2 : 1;
        else if (diff == 2)
            val = ((k & 1) == 1) ? 1 : 0;

        if (val == 0)
            continue;

        seq_t bin = binom(r - 1, k - 1);
        sum += (seq_t)val * bin;
    }

    return sum;
}
