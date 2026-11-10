/*
 * A205866  Positions of multiples of 8 in A204922 (differences of Fibonacci
 *          numbers).
 * https://oeis.org/A205866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th multiple of 8 in A204922, where
 *            A204922 is the triangle T(r,c) = F(r+2) - F(c+1), 1<=c<=r, read
 *            by rows. Computed by a scan using Fibonacci numbers mod 8, which
 *            have period 12
 * Range:     u8 n <= 40, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 8624 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A205866_fib8(idx_t i) {
    static const unsigned t[12] = {0, 1, 1, 2, 3, 5, 0, 5, 5, 2, 7, 1};
    return t[(unsigned)(i % 12)];
}

seq_t a(idx_t n) {
    idx_t count = 0, pos = 0;
    for (idx_t r = 1; ; ++r) {
        unsigned fr = A205866_fib8(r + 2);
        for (idx_t c = 1; c <= r; ++c) {
            ++pos;
            unsigned fc = A205866_fib8(c + 1);
            if (((fr + 8 - fc) & 7u) == 0) {
                ++count;
                if (count == n) return (seq_t)pos;
            }
        }
    }
}
