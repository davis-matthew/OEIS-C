/*
 * A005674  a(n) = 2^(n-1) + 2^[ n/2 ] + 2^[ (n-1)/2 ] - F(n+3).
 * https://oeis.org/A005674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) + 2^floor(n/2) + 2^floor((n-1)/2) - F(n+3), with
 *            a(0..3) = 0
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   two_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/two_pow.h"

static seq_t fib(idx_t n) {
    seq_t x = 0, y = 1;
    for (idx_t i = 0; i < n; i++) {
        seq_t t = x + y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n < 4) return 0;
    seq_t t1 = two_pow(n - 1);
    seq_t t2 = two_pow(n / 2);
    seq_t t3 = two_pow((n - 1) / 2);
    seq_t f = fib(n + 3);
    return t1 + t2 + t3 - f;
}
