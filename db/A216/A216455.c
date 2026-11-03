/*
 * A216455  If n is a perfect fourth power, then replace it with the fourth
 *          root of n.
 * https://oeis.org/A216455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n, unless n is a perfect fourth power k^4, then a(n)=k
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 99 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t int_fourth_root(idx_t n)
{
    idx_t low = 0;
    idx_t high = 65535;           /* 2^16-1: max 4th root of a 64‑bit integer */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        uint64_t m2 = (uint64_t)mid * mid;
        uint64_t m4 = m2 * m2;
        if (m4 == n) return mid;
        if (m4 < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* prevent underflow */
            high = mid - 1;
        }
    }
    return high;                  /* floor of the 4th root */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety for out‑of‑range calls */
    idx_t k = int_fourth_root(n);
    uint64_t k2 = (uint64_t)k * k;
    uint64_t k4 = k2 * k2;
    if (k4 == n)
        return (seq_t)k;
    else
        return (seq_t)n;
}
