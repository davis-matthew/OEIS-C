/*
 * A147644  Triangle read by rows: T(n, k) = binomial(n, k) if n < 3,
 *          otherwise T(n, k) = binomial(n, k) + 2*binomial(n-2, k-1).
 * https://oeis.org/A147644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 178, u32 n <= 644, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Validated: standard, pass, 82 terms through n = 1280 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A147644  – Triangle read by rows (T(n,k)=binomial if n<3, else binomial + 2*binomial(n-2,k-1)).
    The sequence a(n) is the linear traversal of the triangle.  */

/*  The utility binomial function is supplied by the build system.  */
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r such that the first r*(r+1)/2 terms cover n. */
    idx_t r = 0;
    idx_t cum = 0;                /* number of terms before current row r */
    while (cum + r + 1 <= n) {
        cum += r + 1;             /* add the (r+1) terms of the current row */
        ++r;                      /* move to subsequent row */
    }

    /* Column index within the row. */
    idx_t k = n - cum;

    /* Compute T(r,k) according to the definition. */
    if (r < 3) {
        return binomial(r, k);
    } else {
        seq_t val = binomial(r, k);
        if (k > 0 && k < r) {    /* only interior positions get the extra term */
            val += 2 * binomial(r - 2, k - 1);
        }
        return val;
    }
}
