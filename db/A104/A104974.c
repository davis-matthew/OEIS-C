/*
 * A104974  A Fredholm-Rueppel triangle.
 * https://oeis.org/A104974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r, k) = Catalan(r+1-k) mod 2, where r = largest integer
 *            with r*(r+1)/2 <= n and k = n - r*(r+1)/2. Catalan(m) is odd iff
 *            m+1 is a power of two. Therefore a(n) = 1 ⇔ ((j+1) & j) == 0
 *            where j = ( (r+1)*(r+2)/2 ) - n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t row_of(idx_t n) {
    /* binary search for r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    if (n == 0) return 0;
    idx_t lo = 0, hi = 1;
    while ((__int128)hi * (hi + 1) / 2 <= n) ++hi;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    idx_t r = row_of(n);
    __int128 end = (__int128)(r + 1) * (r + 2) / 2;
    idx_t j = (idx_t)(end - n);   /* j >= 1 */
    idx_t t = j + 1;
    return ((t & j) == 0) ? 1 : 0;
}
