/*
 * A215078  Triangle of sums of the first k n-th powers multiplied by
 *          binomial(n,k), read by rows.
 * https://oeis.org/A215078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = binomial(n,k) * sum_{j=1..k} j^n a(n) is the triangle T
 *            read by rows (row 0 has length 1, row 1 length 2, …)
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 60, u64 n <= 144, u128 n <= 367,
 *            big any n
 * Helpers:   big_pow, binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

/* compute the row index r such that r*(r+1)/2 <= idx < (r+1)*(r+2)/2 */
static inline idx_t row_of_idx(idx_t idx) {
    /* use the quadratic formula r = floor((sqrt(8*idx+1)-1)/2) */
    __int128 v = (__int128)8*idx + 1;
    seq_t s = floor_sqrt((seq_t)v);
    idx_t r = ((idx_t)s - 1u) / 2u;

    /* adjust for possible rounding errors */
    while (((__int128)r*(r+1)) / 2 > idx) --r;
    while (((__int128)(r+1)*(r+2)) / 2 <= idx) ++r;
    return r;
}

seq_t a(idx_t n) {
    /* Special case: first element is 0 (T(0,0)) */
    if (n == 0u) return 0;

    idx_t r    = row_of_idx(n);                    /* row index */
    idx_t idx_before = ((idx_t)r*(r+1)) / 2;        /* terms before this row */
    idx_t k    = n - idx_before;                   /* column in the row */

    seq_t sum = 0;
    for (idx_t j = 1; j <= k; ++j) {
        sum += big_pow((seq_t)j, (seq_t)r);
    }

    seq_t bin = binomial(r, k);
    return bin * sum;
}
