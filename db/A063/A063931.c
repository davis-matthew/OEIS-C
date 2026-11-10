/*
 * A063931  Duplicate of A001283.
 * https://oeis.org/A063931
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = T(k, m) = k*(k+m), where n = 2 + (k-2)*(k-1)/2 + (m-1), 1
 *            <= m <= k-1
 * Range:     i8 n <= 24, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 24 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A001283_a(idx_t n) {
    idx_t k = 2;
    idx_t start = 2;
    idx_t m;

    if (n < 2) {
        return 0;
    }
    while (n >= start + (k - 1)) {
        start += k - 1;
        k++;
    }
    m = n - start + 1;
    return (seq_t)k * ((seq_t)k + (seq_t)m);
}

seq_t a(idx_t n) {
    return A001283_a(n);
}
