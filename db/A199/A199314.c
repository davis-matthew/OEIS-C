/*
 * A199314  a(n) = (11*5^n + 1)/4.
 * https://oeis.org/A199314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*5^n + 1)/4
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 985 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t pow5 = ipow((seq_t)5, (seq_t)n);   /* 5^n          */
    seq_t num  = (seq_t)11 * pow5 + (seq_t)1;   /* 11*5^n + 1   */
    return num / (seq_t)4;                     /* divide by 4   */
}
