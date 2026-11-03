/*
 * A005832  Product k^(2^(k-1)), k = 1..n.
 * https://oeis.org/A005832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} k^(2^(k-1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 10 terms through n = 10 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t k = 1; k <= n; k++) {
        idx_t exp = (idx_t)1 << (k - 1);
        seq_t power = ipow_idx((seq_t)k, exp);
        result *= power;
    }
    return result;
}
