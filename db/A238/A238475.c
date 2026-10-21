/*
 * A238475  Rectangular array with all start numbers Me(n, k), k >= 1, for the
 *          Collatz operation ud^(2*n), n >= 1, ending in an odd number, read
 *          by antidiagonals.
 * https://oeis.org/A238475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = Me(row, col) where row = offset, col = s-offset and s is
 *            the minimal diagonal such that triangular(s-1) < n ≤
 *            triangular(s). Me(n,k) = 2^(2*n+1)*k – (5*2^(2*n)+1)/3
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   mul_small, pow2, tri_before
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/mul_small.h"
#include "../utils/tri_before.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* guard for out‑of‑bounds index */

    /* Find the diagonal (s) containing the n‑th term */
    idx_t lo = 2;
    idx_t hi = n * 2;                    /* a safe upper bound for small n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (tri_before(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t s = lo;

    /* Offset within the diagonal (1‑based) */
    idx_t prev_tri = tri_before(s - 1);
    idx_t offset   = n - prev_tri;       /* 1 .. s-1 */

    idx_t row = offset;                  /* row = n in Me(row, col)  */
    idx_t col = s - offset;              /* column  */

    /* Compute Me(row, col) */
    seq_t two_n_pow2 = pow2(2 * row);            /* 2^(2*row)   */
    seq_t two_np1    = pow2(2 * row + 1);        /* 2^(2*row+1) */

    seq_t term_div = mul_small(two_n_pow2, 5);   /* 5*2^(2*row) */
    term_div++;                                 /* +1          */
    term_div /= 3;                               /* (5*2^(2*row)+1)/3 */

    seq_t ans = mul_small(two_np1, (unsigned int)col) - term_div;
    return ans;
}
