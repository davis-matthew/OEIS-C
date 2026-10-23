/*
 * A128489  Triangle read by rows: A000012 * A126988 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A128489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T_{ floor((r-j)/j) + 1 }, * where r is the row of the
 *            triangular array such that T_{r‑1} < n ≤ T_r, * and j = n –
 *            T_{r‑1}. By definition T_k = k*(k+1)/2
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>   /* for 'true' in the loop below */

seq_t a(idx_t n)
{
    /* (1) Find the smallest r with T_r >= n.  T_r = r*(r+1)/2. */
    idx_t low = 1;
    idx_t high = 1;
    while (true) {
        unsigned __int128 tri = (unsigned __int128)high * (high + 1ull) / 2ull;
        if (tri >= n)
            break;
        high <<= 1;     /* double high until it is big enough */
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1ull) / 2ull;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;   /* row number (1‑based) */

    /* (2) Compute j = position in the row (1‑based). */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2ull;
    unsigned __int128 j128 = (unsigned __int128)n - prev_tri;
    idx_t j = (idx_t)j128;

    /* (3) Determine column index k. */
    idx_t k = (r - j) / j + 1;

    /* (4) Return triangular number T_k. */
    seq_t kk = (seq_t)k;
    return (kk * (kk + (seq_t)1ull)) / (seq_t)2ull;
}
