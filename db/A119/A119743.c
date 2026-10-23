/*
 * A119743  Triangle read by rows: row n gives number of matchings of size
 *          0<=k<=n (edges) in the complete graph on 2*n >= 2 vertices.
 * https://oeis.org/A119743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = (2*r)! / ((2*r-2*k)! * k! * 2^k) r
 *            is the row containing the index n: r = ceil( (-3 + sqrt(9 +
 *            8*n)) / 2 ) k is the offset within that row: k = n – (
 *            (r-1)*(r+2)/2 + 1 ) The function below uses the binomial
 *            coefficient and an odd-double-factorial to evaluate T(r,k) wi...
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   _odd_double_factorial, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/_odd_double_factorial.h"

/* Helper: find the row r that contains index n */
static idx_t _row_of_idx(idx_t n)
{
    idx_t low = 1, high = n; /* r never exceeds n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* c(mid) = mid*(mid+3)/2  compare with n
           Using 128-bit arithmetic to avoid overflow. */
        unsigned __int128 lhs = (unsigned __int128)mid * (mid + 3);
        unsigned __int128 rhs = (unsigned __int128)n * 2;
        if (lhs >= rhs)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find row r for index n */
    idx_t r = _row_of_idx(n);

    /* Compute start index of row r: ((r-1)*(r+2))/2 + 1 */
    unsigned __int128 t = (unsigned __int128)(r - 1) * (r + 2);
    idx_t start = (idx_t)(t / 2) + 1;

    /* Offset within the row */
    idx_t k = n - start;   /* 0 <= k <= r */

    /* T(r,k) = binom(2r, 2k) * (2k-1)!! */
    idx_t two_r = r << 1;
    idx_t two_k = k << 1;
    seq_t val = binom(two_r, two_k);
    val *= _odd_double_factorial(k);
    return val;
}
