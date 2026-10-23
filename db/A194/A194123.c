/*
 * A194123  Triangular array: T(n,k)=|C(n+2,k)-C(n+2,k+2)|, 0<=k<=n.
 * https://oeis.org/A194123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(floor((sqrt(8*n+1)-1)/2), n -
 *            floor((sqrt(8*n+1)-1)/2)*(floor((sqrt(8*n+1)-1)/2)+1)/2) where
 *            T(n,k) = |C(n+2,k) - C(n+2,k+2)| (0 ≤ k ≤ n)
 * Range:     u8 n <= 22, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Find smallest r such that (r+1)(r+2)/2 > n, i.e. row index of n in the
   triangular enumeration.  Uses a binary search over r. */
static idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* increase high until triangle count exceeds n */
    while ((high * (high + 1)) / 2 <= n)
    {
        if (high > (idx_t)(~0u >> 1)) /* avoid overflow, though n is small */
            break;
        high <<= 1;
    }
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        idx_t tri = (mid * (mid + 1)) / 2; /* number of terms up to row mid-1 */
        if (tri > n)
            high = mid;
        else
            low = mid + 1;
    }
    return low - 1; /* low is first r with tri > n, so r = low-1 */
}

seq_t a(idx_t n)
{
    /* Determine row r and column k from index n */
    idx_t r = find_row(n);
    idx_t offset = (r * (r + 1)) / 2;
    idx_t k = n - offset;

    /* Compute |C(r+2,k) - C(r+2,k+2)| */
    seq_t c1 = binomial(r + 2, k);
    seq_t c2 = binomial(r + 2, k + 2);

    /* unsigned subtraction in the right direction */
    return (c1 >= c2) ? (c1 - c2) : (c2 - c1);
}
