/*
 * A161890  Numbers such that A010060(n) = A010060(n+9).
 * https://oeis.org/A161890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth nonnegative integer x such that popcount(x) mod 2 =
 *            popcount(x+9) mod 2 (A010060 is the parity of the binary weight,
 *            0 for even, 1 for odd)
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t cnt = 0;
    for (idx_t x = 0; ; ++x) {
        /* parity of popcount(x) */
        bool p1 = (__builtin_popcountll((unsigned long long)x) & 1);
        /* parity of popcount(x+9) */
        bool p2 = (__builtin_popcountll((unsigned long long)(x + 9)) & 1);
        if (p1 == p2) {
            if (cnt == n)
                return (seq_t)x;
            ++cnt;
        }
    }
}
