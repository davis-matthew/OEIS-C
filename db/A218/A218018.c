/*
 * A218018  Triangle, read by rows, where T(n,k) = k!*C(n, k)*11^(n-k) for
 *          n>=0, k=0..n.
 * https://oeis.org/A218018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the row‑major flattening of the triangle T(r,k) =
 *            11^(r–k) * r! / (r–k)!, where r = floor((sqrt(8n+1)–1)/2) and k
 *            = n – r(r+1)/2
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 54, u64 n <= 173, u128 n <= 542,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that cumulative terms up to row r‑1 <= n
       < cumulative terms up to row r.  Use binary search. */
    uint64_t lo = 0, hi = n;
    while (lo < hi) {
        uint64_t mid = (lo + hi) / 2;
        uint64_t cum = (mid + 1) * (mid + 2) / 2;   /* (mid+1)(mid+2)/2 */
        if (cum <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    uint64_t r = lo;                                 /* row index */
    uint64_t prev_cum = r * (r + 1) / 2;             /* terms before row r */
    uint64_t k = n - prev_cum;                       /* column index in row r */

    /* Compute the product r!/(r–k)!  (equals the product of integers
       from r–k+1 to r inclusive). */
    seq_t prod = 1;
    for (uint64_t i = r - k + 1; i <= r; ++i)
        prod = prod * (seq_t)i;

    /* Compute 11^(r–k) */
    seq_t pow11 = seq_pow((seq_t)11, r - k);

    return prod * pow11;
}
