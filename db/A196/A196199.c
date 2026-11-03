/*
 * A196199  Count up from -n to n for n = 0, 1, 2, ... .
 * https://oeis.org/A196199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, easy, frac, look
 * Formula:   a(n) = n - floor(sqrt(n))*(floor(sqrt(n))+1)
 * Range:     i8 n <= 10200, i16 n <= 10200, i32 n <= 10200, i64 n <= 10200,
 *            i128 n <= 10200, big any n
 * Validated: standard, pass, 97 terms through n = 9685 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            if (n == 0)
                return 0;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t t = isqrt_u64(n);
    return (seq_t)n - (seq_t)t * (seq_t)(t + 1);
}
