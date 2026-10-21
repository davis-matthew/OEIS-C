/*
 * A098474  Triangle read by rows, T(n,k) = C(n,k)*C(2*k,k)/(k+1), n >= 0, 0
 *          <= k <= n.
 * https://oeis.org/A098474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(row, k) where * row = floor((sqrt(8*n+1)-1)/2) * k = n
 *            - row*(row+1)/2 * T(n,k) = C(n,k) * C(2k,k) / (k+1) = C(n,k) *
 *            Catalan(k)
 * Range:     u8 n <= 26, u16 n <= 72, u32 n <= 183, u64 n <= 583,
 *            u128 n <= 1935, big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t low = 0;
    uint64_t high = UINT64_MAX;
    uint64_t ans = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* compute the row r: r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    uint64_t r_floor = isqrt_u128(val);
    idx_t r = (r_floor - 1) / 2;

    idx_t cum_before = r * (r + 1) / 2;
    idx_t k = n - cum_before;

    seq_t bin = binomial(r, k);
    seq_t cat = catalan(k);
    return bin * cat;  /* binomial(r,k) * Catalan(k) */
}
