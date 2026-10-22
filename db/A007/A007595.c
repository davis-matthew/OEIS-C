/*
 * A007595  a(n) = C_n / 2 if n is even or ( C_n + C_((n-1)/2) ) / 2 if n is
 *          odd, where C = Catalan numbers (A000108).
 * https://oeis.org/A007595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n)/2 for even n, a(n) = (C(n) + C((n-1)/2))/2 for odd
 *            n, where C = A000108. Fixed-width build: C(n) = Sum_{i=0..n-1}
 *            C(i)*C(n-1-i), halved by symmetry, so no intermediate exceeds
 *            a(n). Big build: C(k+1) = C(k)*(4k+2)/(k+2), C(0) = 1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   a_fixed, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/a_fixed.h"

static seq_t a_big(idx_t n) {
    seq_t c = 1, cn = 0, cm = 0;
    idx_t m = (n - 1) / 2;
    idx_t k;
    for (k = 0; ; k++) {
        if (k == m) cm = c;
        if (k == n) { cn = c; break; }
        c = c * (seq_t)(4 * k + 2) / (seq_t)(k + 2);
    }
    if (n % 2 == 0) return cn / 2;
    return (cn + cm) / 2;
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return a_fixed(n);
    return a_big(n);
}
