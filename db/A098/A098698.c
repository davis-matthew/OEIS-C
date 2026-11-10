/*
 * A098698  Main diagonal of array in A098697.
 * https://oeis.org/A098698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)*A000248(2n-k), where A000248(m) =
 *            Sum_{j=0..m} C(m,j)*(m-j)^j
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-07)
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
    seq_t s = 0;
    seq_t c = 1;
    idx_t k;
    for (k = 0; k <= n; k++) {
        if (k > 0) c = c * (seq_t)(n - k + 1) / (seq_t)k;
        s += c * A000248_a(2 * n - k);
    }
    return s;
}
