/*
 * A216607  The sequence used to represent partition binary diagram as an
 *          array.
 * https://oeis.org/A216607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((1/4)*ceil(sqrt(4*n))^2) - n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t isqrt_u64(uint64_t x) {
    uint64_t low = 0, high = 0xFFFFFFFFULL, ans = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        uint64_t sq = mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safe handling of n=0 */
    uint64_t r = isqrt_u64(n);       /* floor sqrt(n) */
    uint64_t b = n - r * r;          /* remainder n - r^2 */

    seq_t result;
    if (b == 0) {                    /* perfect square */
        result = 0;
    } else if (b <= r) {             /* b in [1, r] */
        result = r - b;
    } else {                         /* b > r */
        result = (2 * r + 1) - b;
    }
    return result;
}
