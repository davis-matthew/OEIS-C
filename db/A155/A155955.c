/*
 * A155955  Triangle read by rows: T(n,k) = (k*n)^k, 0 <= k <= n.
 * https://oeis.org/A155955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k*n)^k where * r = the row index, k = column index * * r
 *            is the unique integer with r*(r+1)/2 <= n < (r+1)*(r+2)/2 * k =
 *            n - r*(r+1)/2 (so 0 <= k <= r) * * This function computes a(n)
 *            by first determining r via a * binary search on the triangular
 *            numbers and then using * seq_pow to comput...
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 33, u64 n <= 64, u128 n <= 151,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Special case: first term of the triangle. */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Find the row r: the largest r such that r*(r+1)/2 <= n. */
    idx_t lo = (idx_t)0;
    idx_t hi = n;                     /* upper bound is n itself     */
    while (lo < hi) {
        /* mid chosen as the higher one to avoid infinite loop */
        idx_t mid = (lo + hi + (idx_t)1) >> 1;
        /* Use unsigned __int128 to avoid overflow. */
        __uint128_t tri = (__uint128_t)mid * (mid + (idx_t)1) / (idx_t)2;
        if (tri <= n) {
            lo = mid;
        } else {
            hi = mid - (idx_t)1;
        }
    }
    idx_t r = lo;                       /* row index */

    /* Column index k */
    idx_t k = n - (__uint128_t)r * (r + (idx_t)1) / (idx_t)2;

    /*  (k*r)^k ; the case k == 0 represents 0^0 which by convention
        is defined as 1 in this sequence.  seq_pow handles the
        exponentiation, using efficient repeated squaring.  */
    if (k == (idx_t)0) {
        return (seq_t)1;                /* (0*n)^0 = 1 */
    }

    seq_t base = (seq_t)k * (seq_t)r;   /* (k*n) */
    return seq_pow(base, k);            /* base^k  */
}
