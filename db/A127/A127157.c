/*
 * A127157  Triangle read by rows: T(n,k) is the number of ordered trees with
 *          n edges and 2k nodes of odd degree (not outdegree; 1 <= k <=
 *          ceiling(n/2)).
 * https://oeis.org/A127157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th term in the triangle * T(n,k) =
 *            2*binomial(3k-1,2k)*binomial(n-1+k,3k-2)/(3k-1) * where k runs
 *            from 1 to ceil(n/2). The sequence a(n) is the * concatenation of
 *            all rows of this triangle, offset starting at 1
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

/*  Total number of terms up to (and including) row m.
 *  Row m has ceil(m/2) entries.  This routine works for m>=0. */
static inline idx_t total_terms_upto(idx_t m)
{
    if (m == 0) return 0;
    if ((m & 1ULL) == 0) {                 /* even row 2t */
        idx_t t = m >> 1;
        return t * (t + 1);
    } else {                               /* odd row 2t‑1 */
        idx_t t = (m + 1) >> 1;
        return t * t;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1 – value undefined otherwise */
        return 0;

    /* Find the row m such that total_terms_upto(m‑1) < n <= total_terms_upto(m) */
    idx_t low = 1, high = 1;
    while (total_terms_upto(high) < n)
        high <<= 1;                /* exponential search to get an upper bound */

    while (low < high) {           /* binary search for the exact row */
        idx_t mid = low + ((high - low) >> 1);
        if (total_terms_upto(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;                 /* row number */

    idx_t prev = total_terms_upto(m - 1);
    idx_t k = n - prev;            /* column index within the row (1‑based) */

    /*  Compute T(m,k) via the provided binomial helper  */
    seq_t part1 = binomial(3 * k - 1, 2 * k);
    seq_t part2 = binomial(m - 1 + k, 3 * k - 2);

    seq_t value = part1 * part2;
    value *= 2;                    /* multiply by 2 */
    seq_t denom = (seq_t)(3 * k - 1);
    value /= denom;                /* exact division */

    return value;
}
