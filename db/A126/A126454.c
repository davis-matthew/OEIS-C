/*
 * A126454  Triangle, read by rows, where T(n,k) = C( C(n+2,3) - C(k+2,3) + 2,
 *          n-k) for n>=k>=0.
 * https://oeis.org/A126454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r and c are the row/column of the (n+1)-st
 *            entry of the triangle read by rows, and T(n,k) = binom( C(n+2,3)
 *            - C(k+2,3) + 2 , n-k ). C(m,3) = m*(m-1)*(m-2)/6
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 27, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binom, triangular_pref
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/triangular_pref.h"

static inline idx_t C3(idx_t x) {
    /* compute x*(x+1)*(x+2)/6 using 128‑bit arithmetic to avoid overflow */
    __int128 v = (__int128)(x + 2) * (x + 1) * x;
    return (idx_t)(v / 6);
}

seq_t a(idx_t n) {
    /* Find the row r such that (r+1)*(r+2)/2 > n  */
    idx_t low = 0, high = 1;
    while (((high + 1) * (high + 2)) / 2 <= n)
        high *= 2;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((mid + 1) * (mid + 2)) / 2 > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = high;

    /* Column position within the row */
    idx_t prev_total = triangular_pref(r);
    idx_t c = n - prev_total;

    idx_t nC3 = C3(r);
    idx_t kC3 = C3(c);
    idx_t bin_arg = nC3 - kC3 + 2;

    /* Return the binomial coefficient */
    return binom(bin_arg, r - c);
}
