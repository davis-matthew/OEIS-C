/*
 * A171375  a(n) = 1+3*10^n+100^n.
 * https://oeis.org/A171375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1+3*10^n+100^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_u64_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t pow10n  = ipow_u64_idx(10, n);
    seq_t pow100n = ipow_u64_idx(100, n);
    seq_t res = seq_mul(pow10n, (seq_t)3);
    res = res + pow100n;
    res = res + 1;
    return res;
}
