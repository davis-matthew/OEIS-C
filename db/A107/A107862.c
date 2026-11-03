/*
 * A107862  Triangle, read by rows, where T(n,k) = C(n*(n-1)/2 - k*(k-1)/2 +
 *          n-k, n-k).
 * https://oeis.org/A107862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = binomial((n-k)*(n+k+1)/2, n-k). Sequence a(n) is the
 *            triangle read by rows, flattened into a single sequence
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 54, u64 n <= 135,
 *            u128 n <= 350, big any n
 * Validated: standard, pass, 71 terms through n = 777 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <sys/types.h>
#include "../utils/binomial.h"

static inline idx_t row_of_index(idx_t n)
{
    /* Binary search for the largest r with r*(r+1)/2 <= n. */
    idx_t low = 0, high = n, mid, tri;
    idx_t r = 0;
    while (low <= high)
    {
        mid = low + (high - low) / 2;
        tri = mid * (mid + 1) / 2;
        if (tri <= n) { r = mid; low = mid + 1; }
        else           high = mid - 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Find row r and column c corresponding to index n.
       row_of_index returns r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = row_of_index(n);
    idx_t c = n - r * (r + 1) / 2;

    /* Compute binomial argument: (r-c)*(r+c+1)/2 choose (r-c) */
    idx_t k   = r - c;               /* equals n-k from formula */
    idx_t N   = (k * (r + c + 1)) / 2;

    return binomial(N, k);
}
