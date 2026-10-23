/*
 * A143940  Triangle read by rows: T(n,k) is the number of unordered pairs of
 *          vertices at distance k in a linear chain of n triangles (i.e.,
 *          joined like VVV..VV; here V is a triangle!), 1 <= k <= n.
 * https://oeis.org/A143940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143940: flattened triangle T(r,k)
   T(r,1)=3r;  T(r,k)=4(r-k+1) for k>1.
   a(n) is the n‑th element read by rows of that triangle. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)         /* sequence is defined for n ≥ 1 */
        return 0;

    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t lo = 1, hi = 1;
    while ((seq_t)hi * (hi + 1) / 2 < (seq_t)n)
        hi <<= 1;                      // exponential search

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t t = (seq_t)mid * (mid + 1) / 2;
        if (t >= (seq_t)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* Column index k (1‑based) within row r */
    idx_t pre = r * (r - 1) / 2;          // elements before row r
    idx_t k   = n - pre;                 // 1 ≤ k ≤ r

    /* Return the value at (r,k) */
    if (k == 1)
        return (seq_t)3 * r;
    else
        return (seq_t)4 * (r - k + 1);
}
