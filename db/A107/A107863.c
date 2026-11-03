/*
 * A107863  Column 1 of triangle A107862; a(n) = binomial(n*(n+1)/2 + n, n).
 * https://oeis.org/A107863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n*(n+3)/2, n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 348 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <sys/types.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t N  = nn * (nn + (seq_t)3) / (seq_t)2;  /* N = n*(n+3)/2 */
    return binomial((idx_t)N, n);
}
