/*
 * A067731  Maximum number of distinct parts in a self-conjugate partition of
 *          n, or 0 if n=2.
 * https://oeis.org/A067731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = r - (s mod 2), where n = r(r+1)/2 + s and 0 <= s <= r;
 *            special case: a(2) = 0
 * Range:     u8 n <= 45, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the exceptional case first */
    if (n == 2) return 0;

    /* Find r such that r(r+1)/2 <= n < (r+1)(r+2)/2.
       Binary search on r (0 <= r <= n).  Use 128‑bit arithmetic for safety. */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tmp = (unsigned __int128)mid * (mid + 1) / 2;
        seq_t tri = (seq_t)tmp;
        if (tri <= (seq_t)n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Compute s = n - r(r+1)/2  (0 <= s <= r) */
    unsigned __int128 tmp_r = (unsigned __int128)r * (r + 1) / 2;
    seq_t tri_r = (seq_t)tmp_r;
    idx_t s = (idx_t)((seq_t)n - tri_r);   /* s fits in idx_t because s <= r */

    /* a(n) = r - (s mod 2) */
    seq_t ans = (seq_t)r - (seq_t)(s & 1);
    return ans;
}
