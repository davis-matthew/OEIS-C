/*
 * A007819  a(n) = Sum_{j=1..n} binomial(n^2, j).
 * https://oeis.org/A007819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..n} binomial(n^2, j), with binomial(n^2, j) =
 *            binomial(n^2, j-1) * (n^2 - j + 1) / j
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 329 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n) {
    idx_t m = n * n;
    seq_t term = 1;
    seq_t result = 0;
    for (idx_t j = 1; j <= n; j++) {
        uint64_t r = (uint64_t)(term % j);
        uint64_t g = gcd_u64(r, j);
        if (g == 0) g = j;
        uint64_t jj = j / g;
        uint64_t f = (m - j + 1) / jj;
        term = (term / g) * f;
        result += term;
    }
    return result;
}
