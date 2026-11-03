/*
 * A234811  a(n) = (6^n - 1)^n.
 * https://oeis.org/A234811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6^n - 1)^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t p6 = ipow_idx((seq_t)6, n);
    seq_t base = p6 - (seq_t)1;
    return ipow_idx(base, n);
}
