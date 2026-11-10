/*
 * A099076  a(n) = A000960(n) mod 3.
 * https://oeis.org/A099076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000960(n+1) mod 3, where A000960(m) starts with m^2 and
 *            for d = m-1, m-2, ..., 1 replaces it by the largest multiple of
 *            d strictly less than it
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n + 1;
    uint64_t result = m * m;
    for (uint64_t d = m - 1; d > 0; d--) {
        uint64_t q = result / d;
        if (result % d == 0) q--;
        result = q * d;
    }
    return (seq_t)(result % 3);
}
