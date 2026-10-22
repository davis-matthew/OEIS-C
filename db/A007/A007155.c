/*
 * A007155  a(n) = (2^(2^n))*(3^(3^n - 2^n)).
 * https://oeis.org/A007155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2^n) * 3^(3^n - 2^n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   a007155_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a007155_pow.h"

seq_t a(idx_t n) {
    uint64_t p2, p3;
    idx_t i;
    if (n > 40) return (seq_t)0; /* value far exceeds any representable size */
    p2 = 1;
    p3 = 1;
    for (i = 0; i < n; i++) {
        p2 *= 2;
        p3 *= 3;
    }
    return a007155_pow((seq_t)2, p2) * a007155_pow((seq_t)3, p3 - p2);
}
