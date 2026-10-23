/*
 * A119245  Triangle, read by rows, defined by: T(n,k) =
 *          (4*k+1)*binomial(2*n+1, n-2*k)/(2*n+1) for n >= 2*k >= 0.
 * https://oeis.org/A119245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th element of the triangle T(n,k) =
 *            (4*k+1)*C(2*n+1,n-2*k)/(2*n+1) read row-wise
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"

/* cumulative number of terms before row r */
static inline idx_t cum_before(idx_t r)
{
    if (r == 0) return 0;
    idx_t m = r - 1; /* last index of previous rows */
    idx_t sum_floor;
    if ((m & 1ULL) == 0) {            /* m even => m = 2*p */
        idx_t p = m / 2;
        sum_floor = p * p;
    } else {                         /* m odd  => m = 2*p + 1 */
        idx_t p = m / 2;
        sum_floor = p * (p + 1);
    }
    /* total elements before row r = r + sum_floor */
    return r + sum_floor;
}

/* Find the row r such that cum_before(r) <= n < cum_before(r+1) */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = n + 1; /* safe upper bound */
    while (low < high) {
        idx_t mid = (low + high) / 2;
        if (cum_before(mid + 1) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* locate row r */
    idx_t r = find_row(n);

    /* offset within the row */
    idx_t k = n - cum_before(r);          /* 0 <= k < 1 + floor(r/2) */

    /* computing T(r,k) */
    /* binomial(2*r+1, r-2*k) */
    seq_t bin = binom((idx_t)(2 * r + 1), (idx_t)(r - 2 * k));

    /* multiplier (4*k + 1) */
    seq_t mult = ((seq_t)4 * (seq_t)k) + (seq_t)1;

    /* numerator and division by (2*r + 1) */
    seq_t num = mult * bin;
    seq_t denom = (seq_t)(2 * r + 1);
    return num / denom;   /* exact division */
}
