/*
 * A214080  a(n) = (floor(sqrt(n)))!
 * https://oeis.org/A214080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (floor(sqrt(n)))!
 * Range:     u8 n <= 35, u16 n <= 80, u32 n <= 168, u64 n <= 440,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 961 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

static idx_t int_floor_sqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq <= n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t r = int_floor_sqrt(n);
    return factorial(r);
}
