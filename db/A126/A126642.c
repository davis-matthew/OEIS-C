/*
 * A126642  a(n) is the number of integers k less than 10^n such that the
 *          decimal representation of k lacks the digits 1,2,3,4 and 5 and at
 *          least one of digits 6,7,8,9.
 * https://oeis.org/A126642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*4^n-6*3^n+4*2^n-1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 980 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n<1, return 0 */

    /* compute powers using the provided helper to avoid overflow */
    seq_t p4 = ipow_u64_idx(4, n);   /* 4^n   */
    seq_t p3 = ipow_u64_idx(3, n);   /* 3^n   */
    seq_t p2 = ipow_u64_idx(2, n);   /* 2^n   */

    /* rearrange to keep all intermediate values non‑negative:
       a(n) = (4*4^n + 4*2^n) - (6*3^n + 1)  */
    seq_t top    = 4 * p4 + 4 * p2;
    seq_t bottom = 6 * p3 + 1;

    return top - bottom;
}
