/*
 * A046236  Cubes which are palindromes in base 6.
 * https://oeis.org/A046236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (6^(n-2) + 1)^3 for n ≥ 3; a(1)=0; a(2)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    seq_t exp = (seq_t)(n - 2);           /* exponent */
    seq_t val = ipow((seq_t)6, exp);   /* 6^(n-2) */
    val += (seq_t)1;                      /* 6^(n-2) + 1 */
    seq_t sq = val * val;                 /* square */
    return sq * val;                      /* cube */
}
