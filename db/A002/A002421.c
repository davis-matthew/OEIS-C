/*
 * A002421  Expansion of (1-4*x)^(3/2) in powers of x.
 * https://oeis.org/A002421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 12*(2*n-4)!/(n!*(n-2)!) for n > 1, via a(n+1) =
 *            a(n)*(4*n-6)/(n+1), a(0) = 1, a(1) = -6, a(2) = 6
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 21, i64 n <= 38, i128 n <= 71,
 *            big any n
 * Helpers:   a002421_gcd_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002421_gcd_u.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return -6;
    seq_t r = 6;
    idx_t k;
    for (k = 2; k < n; k++) {
        /* r = a(k) > 0; a(k+1) = r * (4k-6) / (k+1) */
        uint64_t num = (uint64_t)(4 * k - 6);
        uint64_t den = (uint64_t)(k + 1);
        uint64_t rm = (uint64_t)(long long)(r % (seq_t)den);
        uint64_t g = a002421_gcd_u(den, rm);
        uint64_t h = den / g;
        r = r / (seq_t)g;
        r = r * (seq_t)(num / h);
    }
    return r;
}
