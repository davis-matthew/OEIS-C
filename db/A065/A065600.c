/*
 * A065600  Triangle T(n,k) giving number of Dyck paths of length 2n with
 *          exactly k hills (0 <= k <= n).
 * https://oeis.org/A065600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 230, u64 n <= 702,
 *            u128 n <= 2484, big any n
 * Helpers:   binom, sqrt_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A065600 – flattened triangle of Dyck paths with k hills.
   Each term a(n) corresponds to the entry T(r,k) of the triangle,
   where r is the row number and k the column index within that row:
   r*(r+1)/2 <= n < (r+1)*(r+2)/2,  k = n - r*(r+1)/2.

   Formula for triangle entries (from the OEIS G.f.):

       T(n,k) = 1/(n+1) * sum_{i=0}^{n-k} (-1)^i * (k+i+1) *
                binom(k+i , i) * binom(2*n - k - i , n)

   where 0 <= k <= n, binom(a,b) is the binomial coefficient.
*/

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/sqrt_uint128.h"

/* Main function requested: compute a(n) of A065600. */
seq_t a(idx_t n) {
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    __uint128_t val = (__uint128_t)8 * n + 1;
    idx_t s = sqrt_uint128(val);           /* floor sqrt(8n+1) */
    idx_t r = (s - 1) / 2;                 /* row number */

    /* Compute start index of the row. */
    __uint128_t start = (__uint128_t)r * (r + 1) / 2;
    idx_t k = n - (idx_t)start;            /* column index within row */

    /* Now compute T(r,k) by the closed form formula. */
    seq_t sum = 0;
    for (idx_t i = 0; i <= r - k; ++i) {
        seq_t comb1 = binom(k + i, i);
        seq_t comb2 = binom((idx_t)(2 * r - k - i), r);
        seq_t term = (seq_t)(k + i + 1) * comb1 * comb2;
        if (i & 1) sum -= term;
        else       sum += term;
    }
    seq_t denom = (seq_t)(r + 1);
    seq_t result = sum / denom;
    return result;
}
