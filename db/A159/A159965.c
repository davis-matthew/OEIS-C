/*
 * A159965  Riordan array (1/sqrt(1-4x), (1-2x-(1-3x)c(x))/(x*sqrt(1-4x))),
 *          c(x) the g.f. of A000108.
 * https://oeis.org/A159965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(2*r + k, r + 2*k) where r =
 *            floor((sqrt(8*n+1)-1)/2) (row index) k = n - r*(r+1)/2 (column
 *            index in that row)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* declares seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n) {
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 using binary search. */
    idx_t low = 0;
    idx_t high = n;            /* high can be at most n, safe for 64‑bit n */
    idx_t r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }

    /* Number of terms before row r */
    unsigned __int128 prev = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = (idx_t)(n - (idx_t)prev);   /* column index */

    /* Triangle entry: T(r,k) = binomial(2*r + k, r + 2*k) */
    idx_t a_val = (idx_t)(2 * r + k);
    idx_t b_val = (idx_t)(r + 2 * k);
    return binomial(a_val, b_val);
}
