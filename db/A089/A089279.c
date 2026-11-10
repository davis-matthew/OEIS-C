/*
 * A089279  a(n) = 2 + sum(k=1 to n) [(-1)^k A001511(k)].
 * https://oeis.org/A089279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + Sum_{k=1..n} (-1)^k * A001511(k), where A001511(k) =
 *            1 + (2-adic valuation of k)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A001511_a(idx_t n) {
    seq_t result = 1;
    while ((n & 1) == 0) {
        result++;
        n >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t s = 2;
    idx_t k;
    for (k = 1; k <= n; k++) {
        if (k & 1) {
            s -= 1;
        } else {
            s += A001511_a(k);
        }
    }
    return s;
}
