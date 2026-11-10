/*
 * A066221  Bisection of A001189.
 * https://oeis.org/A066221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001189(2n+1) = t(2n+1) - 1, where t(k) = t(k-1) +
 *            (k-1)*t(k-2), t(0) = t(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 90 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t A001189_a(idx_t n) {
    seq_t prev2 = 1;
    seq_t prev1 = 1;
    idx_t i;
    if (n == 0) return 0;
    for (i = 2; i <= n; i++) {
        seq_t cur = prev1 + (seq_t)(i - 1) * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1 - 1;
}

seq_t a(idx_t n) {
    return A001189_a(2 * n + 1);
}
