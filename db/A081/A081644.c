/*
 * A081644  Integers congruent to 0, 1, 4, 9 or 16 (mod 25) which are not
 *          squares.
 * https://oeis.org/A081644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Integers congruent to 0, 1, 4, 9 or 16 (mod 25) which are not perfect squares.
   Formula: return the n‑th number m such that (m % 25) ∈ {0,1,4,9,16} and m is not a square.
*/
#include <stdint.h>
#include <stdbool.h>

/* Check whether x is a perfect square (for unsigned 64‑bit integers). */
static bool is_square_uint64(uint64_t x)
{
    if (x == 0 || x == 1) return true;
    uint64_t lo = 1, hi = x / 2;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == x) return true;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used (offset is 1) */
    uint64_t count = 0;
    uint64_t m = 1;
    for (;;) {
        uint32_t r = m % 25U;
        if (r == 0 || r == 1 || r == 4 || r == 9 || r == 16) {
            if (!is_square_uint64(m)) {
                ++count;
                if (count == n) return (seq_t)m;
            }
        }
        ++m;
    }
    /* unreachable */
}
