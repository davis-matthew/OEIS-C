/*
 * A189364  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=sqrt(2), t=(1+sqrt(5))/2.
 * https://oeis.org/A189364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*√2⌋ + ⌊n*φ⌋, where φ=(1+√5)/2
 * Range:     u8 n <= 7, u16 n <= 297, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9777 (2026-11-01)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    /* ⌊n*√2⌋ = ⌊√(2*n²)⌋ */
    seq_t s1 = isqrt_seq((seq_t)2 * nn * nn);
    /* ⌊n*φ⌋ = ⌊(n + √(5*n²)) / 2⌋ = ⌊(n + ⌊√(5*n²)⌋) / 2⌋ */
    seq_t s2 = isqrt_seq((seq_t)5 * nn * nn);
    seq_t res = nn + s1 + (nn + s2) / (seq_t)2;
    return res;
}
