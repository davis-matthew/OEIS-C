/*
 * A205377  Least h such that n divides the h-th difference between distinct
 *          odd primes, as ordered in A205376.
 * https://oeis.org/A205377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least h such that n | A205376(h), where A205376(h) =
 *            (2r+1)^2-(2k+1)^2 = 4(r-k)(r+k+1) (r = row, k = 0-based column);
 *            computed by direct search over the triangle
 * Range:     u8 n <= 46, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    uint64_t m = (uint64_t)n;
    uint64_t h = 0;
    for (uint64_t r = 1;; r++) {
        for (uint64_t k = 0; k < r; k++) {
            h++;
            unsigned __int128 d = (unsigned __int128)(4 * (r - k)) * (unsigned __int128)(r + k + 1);
            if ((uint64_t)(d % m) == 0) return (seq_t)h;
        }
    }
}
