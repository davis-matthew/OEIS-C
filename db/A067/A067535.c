/*
 * A067535  Smallest squarefree number >= n.
 * https://oeis.org/A067535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + A081221(n) = smallest squarefree number >= n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_squarefree_uint64(uint64_t x) {
    if (x < 2) return true;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            uint64_t cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
                if (cnt > 1) return false;
            }
        }
    }
    return true;
}

seq_t a(idx_t n) {
    for (idx_t m = n; ; ++m) {
        if (is_squarefree_uint64((uint64_t)m))
            return (seq_t)m;
    }
}
