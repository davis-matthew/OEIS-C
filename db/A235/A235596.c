/*
 * A235596  Second column of triangle in A235595.
 * https://oeis.org/A235596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000248(n-1) - 1, where A000248(m) = Sum_{k=0..m}
 *            C(m,k)*(m-k)^k
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 179 (2026-11-07)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

static seq_t A000248_a(idx_t n)
{
    seq_t s = 0;
    seq_t c = 1;
    idx_t k;
    for (k = 0; k <= n; k++) {
        if (k > 0) c = c * (seq_t)(n - k + 1) / (seq_t)k;
        s += c * ipow_idx((seq_t)(n - k), k);
    }
    return s;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    return A000248_a(n - 1) - 1;
}
