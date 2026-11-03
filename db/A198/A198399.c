/*
 * A198399  a(n) = 9^n - 9*n.
 * https://oeis.org/A198399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - 9*n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 939 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t power   = ipow((seq_t)9, (seq_t)n);     /* 9^n   */
    seq_t subtract = (seq_t)9 * (seq_t)n;            /* 9*n   */
    return power - subtract;
}
