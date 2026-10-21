/*
 * A236538  Triangle read by rows: T(n,k) = (n+1)*2^(n-2)+(k-1)*2^(n-1) for 1
 *          <= k <= n.
 * https://oeis.org/A236538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = row such that r(r+1)/2 >= n, k = n -
 *            (r-1)r/2, T(r,k) = (r+1)*2^(r-2) + (k-1)*2^(r-1) (for r>=2),
 *            with T(1,1) = 1
 * Range:     u8 n <= 20, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

static inline idx_t find_row(idx_t n)
{
    idx_t low = 1;
    idx_t high = ((idx_t)1 << 33);    /* sufficient for any 64‑bit idx_t */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 sum = (__int128)mid * (mid + 1) / 2;
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* safety for out of bounds */
    if (n == 1) return (seq_t)1;    /* T(1,1) = 1 */

    idx_t r = find_row(n);
    idx_t prev_sum = (__int128)(r - 1) * r / 2;
    idx_t k = n - prev_sum;         /* k is 1‑based within row r */

    /* Compute T(r,k) for r >= 2 */
    seq_t term1 = (seq_t)(r + 1) * pow2(r - 2);
    seq_t term2 = (seq_t)(k - 1) * pow2(r - 1);
    return term1 + term2;
}
