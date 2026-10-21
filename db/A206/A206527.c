/*
 * A206527  3^n concatenated with itself.
 * https://oeis.org/A206527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concat(3^n, 3^n) = 3^n * (10^d + 1) where d =
 *            digit_len(3^n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   digit_len, pow10, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow3.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    seq_t base = pow3(n);           /* 3^n */
    int d = digit_len(base);       /* number of decimal digits of base */
    seq_t t = pow10(d);            /* 10^d */
    return base * t + base;        /* (10^d + 1) * base */
}
