/*
 * A007440  Reversion of g.f. for Fibonacci numbers 1, 1, 2, 3, 5, ....
 * https://oeis.org/A007440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   (n+3)*a(n+2) = -(2*n+3)*a(n+1) - 5*n*a(n), a(1) = 1, a(2) = -1
 * Range:     i8 n <= 11, i16 n <= 18, i32 n <= 34, i64 n <= 63, i128 n <= 119,
 *            big any n
 * Helpers:   a007440_big, from_i128, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/from_i128.h"
#include "../utils/a007440_big.h"

/* Fixed-width builds: 128-bit arithmetic. Each step divides p and pp by d
   first, so the only exact division is on small remainders. The big
   products are summed modulo 2^128, which is exact whenever the result
   fits. */
static seq_t a007440_fixed(idx_t n) {
    __int128 pp = 1, p = -1;
    for (idx_t k = 1; k + 2 <= n; k++) {
        __int128 c1 = (__int128)k * 2 + 3;
        __int128 c2 = (__int128)k * 5;
        __int128 d = (__int128)k + 3;
        __int128 q1 = p / d, r1 = p % d;
        __int128 q2 = pp / d, r2 = pp % d;
        __int128 s = (c1 * r1 + c2 * r2) / d;
        unsigned __int128 t = (unsigned __int128)c1 * (unsigned __int128)q1
                            + (unsigned __int128)c2 * (unsigned __int128)q2
                            + (unsigned __int128)s;
        t = (unsigned __int128)0 - t;
        pp = p;
        p = (__int128)t;
    }
    return from_i128(p);
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return -1;
    if (seq_is_fixed_width()) return a007440_fixed(n);
    return a007440_big(n);
}
