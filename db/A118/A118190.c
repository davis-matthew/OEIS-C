/*
 * A118190  Triangle T(n,k) = 5^(k*(n-k)) for n >= k >= 0, read by rows.
 * https://oeis.org/A118190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 5^(k*(r-k)), where r = floor((sqrt(1+8n)-1)/2) and k = n
 *            - r*(r+1)/2
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 38, u64 n <= 69, u128 n <= 126,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (1) {
        __int128 next = (__int128)(r + 1) * (r + 2) / 2;
        if (next > n) break;
        r++;
    }

    /* Compute k and exponent */
    idx_t tri = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t k = n - tri;

    /* Use 128‑bit intermediate to avoid overflow for large values */
    __int128 exp128 = (__int128)k * (r - k);
    idx_t exp = (idx_t)exp128;

    return power5(exp);
}
