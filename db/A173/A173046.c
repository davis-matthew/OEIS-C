/*
 * A173046  Triangle T(n, k, q) = binomial(n, k) + q^n*binomial(n-2, k-1) - 1
 *          with T(n, 0) = T(n, n) = 1 and q = 2, read by rows.
 * https://oeis.org/A173046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k, 2) where n = offset_row(r)+k T(r,k,2) =
 *            binomial(r,k) + 2^r * binomial(r-2,k-1) - 1, with T(r,0) =
 *            T(r,r) = 1
 * Range:     u8 n <= 22, u16 n <= 59, u32 n <= 196, u64 n <= 643,
 *            u128 n <= 2307, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External utilities */
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/pow2.h"    /* seq_t pow2(idx_t exp); */
#include <stdint.h>          /* for __int128 */

static void get_row_col(idx_t n, idx_t *r, idx_t *k)
{
    /* Find the largest r such that r(r+1)/2 <= n */
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    *r = lo;
    __int128 start = (__int128)lo * (lo + 1) / 2;
    *k = n - (idx_t)start;
}

seq_t a(idx_t n)
{
    idx_t r, k;
    get_row_col(n, &r, &k);

    /* Border values of the triangle are always 1 */
    if (k == 0 || k == r)
        return (seq_t)1;

    /* Compute T(r,k,2) as defined */
    seq_t term = binom(r, k) + pow2(r) * binom(r - 2, k - 1) - (seq_t)1;
    return term;
}
