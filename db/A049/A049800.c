/*
 * A049800  Triangular array T, read by rows: T(n,k) = (n+1) mod
 *          floor((k+1)/2), k = 1..n and n >= 1.
 * https://oeis.org/A049800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r+1) mod floor((k+1)/2), where (r,k) is the row/column
 *            position of n in the triangular array T(n,k) = (n+1) mod
 *            floor((k+1)/2)
 * Range:     u8 n <= 4950, u16 n <= 4950, u32 n <= 4950, u64 n <= 4950,
 *            u128 n <= 4950, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n and (r-1)*r/2 < n.  */
    /* Estimate r using the closed‑form inverse of the triangular number. */
    double rd = sqrt(8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((rd - 1.0) * 0.5);

    /* Correct the estimate in case of rounding error. */
    while ((unsigned __int128)r * (r + 1) / 2 < n) r++;
    while (r > 0 && (unsigned __int128)(r - 1) * r / 2 >= n) r--;

    /* Column k (1‑based) in the row r. */
    idx_t prev_count = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - prev_count;          /* 1 <= k <= r */

    /* Compute the divisor floor((k+1)/2). */
    idx_t divisor = (k + 1) / 2;        /* divisor >= 1 */

    /* Compute the value as (r+1) mod divisor. */
    seq_t val = (seq_t)(r + 1) % (seq_t)divisor;

    return val;
}
