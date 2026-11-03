/*
 * A162784  a(n) = (A048883(n)+1)/2.
 * https://oeis.org/A162784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^(popcount(n)) + 1)/2
 * Range:     u8 n <= 62, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 67 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (A048883(n)+1)/2 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Count 1-bits in n (population count) */
    idx_t pc = __builtin_popcountll((unsigned long long)n);

    /* 3^pc */
    seq_t p = ipow_u64_idx(3, pc);

    /* (p + 1) / 2 */
    return (p + 1) / 2;
}
