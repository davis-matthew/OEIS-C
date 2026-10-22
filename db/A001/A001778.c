/*
 * A001778  Lah numbers: a(n) = n!*binomial(n-1,5)/6!.
 * https://oeis.org/A001778
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*n*(n-1)/(n-6), a(6) = 1 (i.e. a(n) =
 *            n!*binomial(n-1,5)/6!)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   a001778_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001778_gcd.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t i;
    if (n < 6) return 0;
    for (i = 7; i <= n; i++) {
        idx_t d = i - 6;
        idx_t x = i, y = i - 1, g;
        g = a001778_gcd(x, d);
        x /= g;
        d /= g;
        g = a001778_gcd(y, d);
        y /= g;
        d /= g;
        /* now gcd(x*y, d) = 1, so d divides result exactly */
        result /= (seq_t)d;
        result *= (seq_t)x;
        result *= (seq_t)y;
    }
    return result;
}
