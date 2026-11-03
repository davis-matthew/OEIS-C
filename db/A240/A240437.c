/*
 * A240437  Number of non-palindromic n-tuples of 5 distinct elements.
 * https://oeis.org/A240437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - 5^ceil(n/2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1, behavior for 0 not used */

    idx_t half = (n + 1) / 2;      /* ceil(n/2) */
    seq_t p_full = ipow_u64_idx(5, n);   /* 5^n */
    seq_t p_half = ipow_u64_idx(5, half);/* 5^ceil(n/2) */
    return p_full - p_half;         /* difference is the required value */
}
