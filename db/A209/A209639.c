/*
 * A209639  Bisection of A209859.
 * https://oeis.org/A209639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A209859(2n+1); A209859 rewrites binary from the top: 1 ->
 *            1, 0+1 -> 0
 * Range:     u8 n <= 254, u16 n <= 65534, u32 n <= 65535, u64 n <= 65535,
 *            u128 n <= 65535, big any n
 * Validated: standard, pass, 115 terms through n = 9984 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n * 2 + 1;
    int msb = 63 - __builtin_clzll((unsigned long long)m);
    seq_t out = 0;
    int i = msb;
    while (i >= 0) {
        if ((m >> i) & 1) {
            out = (out << 1) | 1;
            i--;
        } else {
            while (i >= 0 && !((m >> i) & 1))
                i--;
            out = out << 1;
            i--;
        }
    }
    return out;
}
