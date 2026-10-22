/*
 * A007226  a(n) = 2*det(M(n; -1))/det(M(n; 0)), where M(n; m) is the n X n
 *          matrix with (i,j)-th element equal to 1/binomial(n + i + j + m,
 *          n).
 * https://oeis.org/A007226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(3n)!/(n!*(n+1)!*... ) via recurrence
 *            2*(n+1)*(2*n-1)*a(n) = 3*(3*n-1)*(3*n-2)*a(n-1), a(0) = 2; i.e.
 *            a(n) = (2/(n+1))*binomial(3*n, n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   a007226_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a007226_gcd.h"

seq_t a(idx_t n) {
    seq_t v = 2;
    idx_t k;
    for (k = 1; k <= n; k++) {
        uint64_t num = 3 * (3 * (uint64_t)k - 1) * (3 * (uint64_t)k - 2);
        uint64_t den = 2 * ((uint64_t)k + 1) * (2 * (uint64_t)k - 1);
        uint64_t g = a007226_gcd(num, den);
        num /= g;
        den /= g;
        /* den divides a(k-1) since a(k) is an integer and gcd(num, den) = 1 */
        v = v / (seq_t)den;
        v = v * (seq_t)num;
    }
    return v;
}
