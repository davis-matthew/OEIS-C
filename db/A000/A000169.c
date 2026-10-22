/*
 * A000169  Number of labeled rooted trees with n nodes: n^(n-1).
 * https://oeis.org/A000169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, core, nonn, nice
 * Formula:   a(n) = n^(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    seq_t result = 1;
    idx_t e = n - 1;
    while (e > 0) {
        if (e & 1)
            result *= base;
        e >>= 1;
        if (e > 0)
            base *= base;
    }
    return result;
}
