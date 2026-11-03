/*
 * A203303  Vandermonde determinant of the first n terms of (1,2,4,8,16,...).
 * https://oeis.org/A203303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{0 <= i < j <= n-1} (2^j - 2^i)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 22 terms through n = 22 (2026-11-01)
 * Helpers:   ipow_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the closed form:
   a(n) = 2^(n*(n-1)*(n-2)/6) * Prod_{k=1}^{n-1} (2^k-1)^(n-k)   */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* exponent for the initial power of two factor */
    idx_t exp = (n * (n - 1) * (n - 2)) / 6;
    seq_t result = pow2(exp);

    for (idx_t k = 1; k < n; ++k) {
        seq_t term   = pow2(k) - (seq_t)1;   /* 2^k - 1  */
        idx_t  e     = n - k;                /* exponent   */
        result = result * ipow_idx(term, e);
    }
    return result;
}
