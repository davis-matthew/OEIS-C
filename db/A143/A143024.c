/*
 * A143024  Triangle read by rows: T(n,k) is the number of non-crossing
 *          connected graphs on n nodes on a circle having root (a
 *          distinguished node) of degree 1 and having k edges (n >= 2, 1 <= k
 *          <= 2n-4).
 * https://oeis.org/A143024
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A143024: a(n) is the n‑th entry of the triangle read row‑wise.
   T(n,k) = 2*binomial(k-2 , n-3) * binomial(3n-5 , 2n-k-4) / (n-2)
   for n≥3, 2≤k≤2n-4; T(2,1)=1, all other entries are 0. 
   The sequence starts with a(2)=1, a(3)=0, a(4)=2, ...           */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* offset 2: a(2) is the first listed term */
    if (n < 2) return 0;
    if (n == 2) return 1;                /* T(2,1) = 1                     */

    /* m = zero‑based index in the flat sequence                     */
    idx_t m = n - 2;

    /* Find minimal t such that t*(t+1) >= m.  t = r-2,  r = row number */
    idx_t low = 0, high = 1;
    while ((__int128)high * (high + 1) < m) {
        high <<= 1;                      /* exponential search for upper bound */
    }
    idx_t lo = high >> 1;
    idx_t hi = high;
    while (lo < hi) {                   /* binary search in [lo,hi]          */
        idx_t mid = lo + (hi - lo) / 2;
        if ((__int128)mid * (mid + 1) >= m) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    idx_t t = hi;                       /* t = r-2                          */
    idx_t r = t + 2;                    /* row index (≥3)                    */

    /* Compute the position within the row (0‑based)                     */
    __int128 prevC_big;
    if (t == 0) {                       /* only occurs for r==2 (handled)   */
        prevC_big = 0;
    } else {
        prevC_big = (__int128)(t - 1) * t + 1;  /* C(r-1) = 1 + (t-1)t          */
    }
    __int128 pos_big = (__int128)m - prevC_big; /* 0 ≤ pos < 2t                  */
    idx_t pos = (idx_t)pos_big;
    idx_t k = pos + 1;                  /* k is 1‑based column index      */

    /* The first r-2 entries of each row are zero                       */
    if (k < r - 1) return 0;

    /* Evaluate the multinomial formula for T(r,k)                       */
    seq_t binom1 = binom(k - 2, r - 3);             /* binom(k-2 , r-3) */
    seq_t binom2 = binom(3 * r - 5, 2 * r - k - 4);/* binom(3r-5 , 2r-k-4)*/
    seq_t num = 2 * binom1 * binom2;                /* numerator      */
    seq_t den = r - 2;                               /* denominator   */
    return num / den;
}
