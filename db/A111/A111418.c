/*
 * A111418  Right-hand side of odd-numbered rows of Pascal's triangle.
 * https://oeis.org/A111418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 152, u64 n <= 594,
 *            u128 n <= 2210, big any n
 * Validated: standard, pass, 64 terms through n = 7192 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A111418
 *  a(n) = value of the triangle T(r,k) with
 *         T(r,k) = C(2*r+1, r-k)
 *  where r = row_of_index(n)
 *        k = n - r*(r+1)/2
 *  i.e. the nth element of the flattened triangle
 *  T(r,0), T(r,1), …, T(r,r) in row order.
 *
 *  The sequence starts:
 *  a(0)=1, a(1)=3, a(2)=1, a(3)=10, a(4)=5, a(5)=1, ...
 *
 *  Implementation uses helper functions:
 *    - row_of_index(idx_t n)   // from utils/row_of_index.h
 *    - binom(idx_t N, idx_t K) // from utils/binom.h
 *
 *  Both functions are provided by the test harness and work
 *  with the build-specific definitions of seq_t and idx_t.
 */

#include <stdbool.h>
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
    /* Determine which row contains the global index n */
    idx_t r = row_of_index(n);

    /* Position within that row (0-based) */
    idx_t offset = n - (r * (r + 1) / 2);

    /* Column index k in the triangle T(r, k) = C(2r+1, r-k) */
    idx_t k = r - offset;

    /* Compute the binomial coefficient  C(2r+1, r-k) */
    return binomial(2 * r + 1, k);
}
