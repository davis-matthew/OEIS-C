/*
 * A007642  Numbers k such that 2*k^2 +29 is composite.
 * https://oeis.org/A007642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th k >= 1 such that 2*k^2 + 29 is composite
 *            (trial division)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool is_composite_u64(uint64_t m) {
    if (m < 4) return false;
    if (m % 2 == 0 || m % 3 == 0) return true;
    for (uint64_t i = 5; i * i <= m; i += 6) {
        if (m % i == 0 || m % (i + 2) == 0) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 0;
    while (count < n) {
        k++;
        uint64_t v = 2 * k * k + 29;
        if (is_composite_u64(v)) count++;
    }
    return (seq_t)k;
}
