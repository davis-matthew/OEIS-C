/*
 * A108283  Triangle read by rows, generated from (..., 3, 2, 1).
 * https://oeis.org/A108283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A108283 – triangle read by rows.
 *
 *  Formula:
 *      Let T(n,k) (1 ≤ k ≤ n) be defined by
 *          T(n,n) = n*(n+1)/2          (last column of each row)
 *          T(n,k) = ( 1 + (n-k+1)^k * (n*k - k*k - 1) ) / (n-k)^2
 *      for k < n.
 *
 *  The sequence a(n) is the row‑wise enumeration of this triangle:
 *      a(1)   = T(1,1)
 *      a(2)   = T(2,1), a(3) = T(2,2)
 *      a(4)   = T(3,1), a(5) = T(3,2), a(6) = T(3,3)
 *      …
 *
 *  The implementation below follows exactly this definition and
 *  works for any integer size of seq_t (including arbitrary precision).
 *  It performs only multiplication, addition, subtraction and integer
 *  division, all in the seq_t type.  The algorithm is O(r) per call,
 *  where r is the row index (≤ n for n ≤ 10000), well under the time
 *  limit for the required range.
 */

#include <stdint.h>   /* for uint64_t */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

static idx_t triangular(idx_t m)
{
    /* m*(m+1)/2 fits in idx_t for the required range */
    return (idx_t)((seq_t)m * (m + 1) / 2);
}

seq_t a(idx_t n)
{
    if (n == 0) { /* offset is 1 – n<1 never requested */
        return 0;
    }

    /* Find the row r such that triangular(r-1) < n ≤ triangular(r) */
    idx_t low = 1, high = 1;
    while (triangular(high) < n) {
        /* growth factor of 2 is sufficient, high will stay far below UINT64_MAX */
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column index within this row (1‑based) */
    idx_t c = n - triangular(r - 1);

    /* If it is the last column, the value is the triangular number */
    if (c == r) {
        return (seq_t)r * (r + 1) / 2;
    }

    /* Otherwise use the explicit formula */
    seq_t r_seq = (seq_t)r;
    seq_t c_seq = (seq_t)c;

    /* (r - c + 1)^c */
    seq_t base = r - c + 1;
    seq_t pow_val = pow_seq(base, c);

    /* (n*k - k^2 - 1) */
    seq_t nk = r_seq * c_seq;
    seq_t kk = c_seq * c_seq;
    seq_t temp = nk - kk - 1;

    /* numerator = 1 + pow_val * temp */
    seq_t numerator = 1 + pow_val * temp;

    /* denominator = (n - k)^2 */
    seq_t diff = r_seq - c_seq;
    seq_t denominator = diff * diff;

    /* division is exact by definition of the sequence */
    return numerator / denominator;
}
