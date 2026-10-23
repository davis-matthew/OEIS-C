/*
 * A174044  Triangle T(n, k, q) = Sum_{j=0..10} q^j * floor(
 *          binomial(n+1,k)*binomial(n-1,k-1)/(2^j*(n+1)) ) for q = 2, read by
 *          rows.
 * https://oeis.org/A174044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   T(n,k) = Sum_{j=0..10} 2^j * floor( binom(n+1,k)*binom(n-1,k-1)
 *            / (2^j*(n+1)) ) with q = 2. The sequence a(n) is read by rows:
 *            a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), ... Offset: 1
 * Range:     u8 n <= 17, u16 n <= 59, u32 n <= 180, u64 n <= 646,
 *            u128 n <= 1275, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* compute a single triangular entry T(n,k) with q = 2.
   n and k are 1‑based; 1 <= k <= n.   */
static seq_t tri_T(idx_t n, idx_t k)
{
    /* Use the identity:
          binom(n+1,k)*binom(n-1,k-1)/(n+1)
        = binom(n,k-1)*binom(n-1,k-1)/k
     which is an integer. */
    seq_t a = binom(n, k - 1);        /* binom(n , k-1) */
    seq_t b = binom(n - 1, k - 1);    /* binom(n-1 , k-1) */
    seq_t prod = a * b;               /* exact for the build type */
    seq_t y = prod / (seq_t)k;        /* integer division – exact */

    /* Sum 2^j * floor(y / 2^j) for j = 0..10.
       floor(y / 2^j) is just a right shift.  */
    seq_t res = 0;
    for (int j = 0; j <= 10; ++j) {
        seq_t part = (y >> j) << j;   /* = 2^j * floor(y/2^j) */
        res += part;
    }
    return res;
}

/* sequence function a(idx_t n) */
seq_t a(idx_t n)
{
    if (n == 0)           /* safety – offset starts at 1 */
        return 0;

    /* Find row r such that r(r+1)/2 >= n.
       r is 1‑based.  Simple incremental search is cheap for
       n up to about 10⁶ and satisfies the timing requirement. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* The index of the first element in row r in the linear layout. */
    idx_t prev_row_start = (r - 1) * r / 2;

    /* Column k = n - first index of row r + 1 (1‑based). */
    idx_t k = n - prev_row_start;

    return tri_T(r, k);
}
