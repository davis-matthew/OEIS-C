/*
 * A004096  Catalan numbers written backwards.
 * https://oeis.org/A004096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004086(A000108(n)), with A000108(n) =
 *            A000108(n-1)*2*(2n-1)/(n+1), A000108(0) = 1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 19, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   small_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/small_gcd_u64.h"

static seq_t catalan(idx_t n) {
    seq_t c = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        uint64_t t = 2 * (2 * (uint64_t)k - 1);
        uint64_t d = (uint64_t)k + 1;
        uint64_t g = small_gcd_u64(t, d);
        t /= g;
        d /= g;
        /* d is coprime to t and divides c*t, so d divides c exactly */
        c /= d;
        c *= t;
    }
    return c;
}

static seq_t reverse_digits(seq_t num) {
    seq_t reversed = 0;
    while (num > 0) {
        reversed = reversed * 10 + (num % 10);
        num /= 10;
    }
    return reversed;
}

seq_t a(idx_t n) {
    return reverse_digits(catalan(n));
}
