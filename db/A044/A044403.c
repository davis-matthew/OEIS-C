/*
 * A044403  Numbers n such that string 7,1 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate through integers and select those that contain the
   decimal substring "71" while the preceding integer does not. */
#include <stdbool.h>

static inline bool has_71(idx_t x)
{
    idx_t prev = (idx_t)-1;
    while (x > 0) {
        idx_t d = x % 10;
        if (prev == 1 && d == 7) return true;
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    for (idx_t x = 1; ; ++x) {
        if (!has_71(x)) continue;          // must contain "71"
        if (has_71(x - 1)) continue;        // previous number must not contain "71"
        ++cnt;
        if (cnt == n) return (seq_t)x;
    }
}
