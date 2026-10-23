/*
 * A013622  Triangle of coefficients in expansion of (3+5*x)^n.
 * https://oeis.org/A013622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = binomial(r,k) * 3^(r-k) * 5^k where r =
 *            floor((sqrt(8*n+1)-1)/2) is the largest index with r(r+1)/2 ≤ n
 *            (row number) and k = n - r(r+1)/2 is the column within that row
 *            (0‑based)
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 82, u64 n <= 286,
 *            u128 n <= 1013, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t (used by idx_t) */

/* Find largest r such that r*(r+1)/2 <= n */
static idx_t
find_row(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* exponential search to bound hi */
    while (( (__int128)hi * (hi + 1) ) / 2 <= n) {
        lo = hi;
        if (hi == (~(idx_t)0) >> 1) {   /* avoid overflow */
            hi = (~(idx_t)0);
            break;
        }
        hi <<= 1;
    }
    /* binary search between lo and hi */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    /* Determine row r and column k for the n‑th entry */
    idx_t r = find_row(n);
    idx_t prefix = (idx_t)((__int128)r * (r + 1) / 2);   /* elements before row r */
    idx_t k = n - prefix;                               /* column index (0≤k≤r) */

    /* Compute binomial coefficient binomial(r,k) */
    seq_t coeff = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        coeff = coeff * (seq_t)(r - i + 1);
        coeff = coeff / (seq_t)i;   /* exact division */
    }

    /* Compute 3^(r-k) */
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < r - k; ++i)
        pow3 = pow3 * (seq_t)3;

    /* Compute 5^k */
    seq_t pow5 = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        pow5 = pow5 * (seq_t)5;

    /* Result = coeff * pow3 * pow5 */
    seq_t result = coeff * pow3;
    result = result * pow5;
    return result;
}
