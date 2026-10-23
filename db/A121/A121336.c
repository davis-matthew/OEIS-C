/*
 * A121336  Triangle, read by rows, where T(n,k) = C( n*(n+1)/2 + n-k + 2,
 *          n-k), for n>=k>=0.
 * https://oeis.org/A121336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading triangle T(n,k) by rows, where *
 *            T(n,k) = C( n*(n+1)/2 + n-k + 2, n-k ). * In terms of the linear
 *            index n, let r be the row such that * offset(r) = r*(r+1)/2 <= n
 *            < offset(r+1). Then k = n - offset(r) * and a(n) = C( r*(r+1)/2
 *            + r-k + 2, r-k ). This implementation * co...
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t low = 0, high = n + 1;          /* high is exclusive */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;

    /* Linear offset of row r */
    unsigned __int128 offset_128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t offset = (idx_t)offset_128;

    idx_t k = n - offset;         /* column index within the row */
    idx_t d = r - k;              /* choose argument (n-k) */

    unsigned __int128 base_128 =
        (unsigned __int128)r * (r + 1) / 2 + (unsigned __int128)d + 2;
    idx_t base = (idx_t)base_128;

    return binom(base, d);
}
