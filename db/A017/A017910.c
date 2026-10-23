/*
 * A017910  Powers of sqrt(2) rounded down.
 * https://oeis.org/A017910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2^(n/2)) (powers of sqrt(2) rounded down)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 154, big any n
 * Helpers:   floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Even n: sqrt(2)^n = 2^(n/2) exactly */
    if ((n & 1) == 0)
        return pow2(n >> 1);

    /* Odd n: a(n) = floor(sqrt(2^n)).
       For n < 128 we can compute this safely with 128‑bit arithmetic
       (no big integer allocation).  For larger n, we fall back to
       pow2 / floor_sqrt which works with arbitrary-precision seq_t.*/
    if (n < 128) {
        /* limit = 2^n  (128-bit) */
        unsigned __int128 limit = (unsigned __int128)1 << n;

        /* Search y in [2^k, 2^(k+1)-1] where k = (n-1)/2 */
        uint64_t k1 = (n - 1) / 2;      /* floor(n/2) */
        uint64_t k2 = (n + 1) / 2;      /* ceil(n/2)  */
        uint64_t low  = ((uint64_t)1 << k1);
        uint64_t high = (k2 == 64) ? UINT64_MAX
                                   : ((uint64_t)1 << k2) - 1;

        uint64_t ans = low;
        while (low <= high) {
            uint64_t mid   = low + ((high - low) >> 1);
            unsigned __int128 mid2 = (unsigned __int128)mid * mid;
            if (mid2 <= limit) {
                ans = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return (seq_t)ans;
    } else {
        /* For large n use generic big‑int facilities.           */
        seq_t t = pow2(n);      /* 2^n as big integer */
        return floor_sqrt(t);
    }
}
