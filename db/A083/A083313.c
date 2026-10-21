/*
 * A083313  a(0)=1; a(n) = 3^n - 2^(n-1) for n >= 1.
 * https://oeis.org/A083313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n - 2^(n-1) for n >= 1, a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   power_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/power_uint.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t p3 = power_uint(3, n);          /* 3^n */
    seq_t p2 = power_uint(2, n - 1);      /* 2^(n-1) */
    return p3 - p2;
}
