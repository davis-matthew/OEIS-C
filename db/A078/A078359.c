/*
 * A078359  Number of ways to write n as sum of a positive square and a
 *          positive cube.
 * https://oeis.org/A078359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (s > 0, c > 0) : n = s^2 + c^3 }
 * Range:     u8 n <= 33, u16 n <= 8512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 8799 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether a non‑negative seq_t value is a perfect square.
   It uses a binary search on seq_t without relying on external
   utilities.  The routine is valid for any seq_t that supports
   the basic arithmetic operators. */
static inline bool is_square_seq(seq_t v)
{
    seq_t low = 0;
    seq_t high = v;
    seq_t ans = 0;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 == v)
            return true;
        if (mid2 < v) {
            low = mid + 1;
            ans = mid;
        } else {
            high = mid - 1;
        }
    }
    return ans * ans == v;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t c = 1; ; ++c) {
        seq_t cube = (seq_t)c * (seq_t)c * (seq_t)c;
        if (cube >= (seq_t)n)
            break;
        seq_t rest = (seq_t)n - cube;
        if (is_square_seq(rest))
            ++count;
    }
    return count;
}
