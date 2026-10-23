/*
 * A189364  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=sqrt(2), t=(1+sqrt(5))/2.
 * https://oeis.org/A189364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*√2⌋ + ⌊n*φ⌋, where φ=(1+√5)/2
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 5032, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    /* ⌊n*√2⌋ = ⌊√(2*n²)⌋ */
    seq_t s1 = sqrt_floor((seq_t)2 * nn * nn);
    /* ⌊n*φ⌋ = ⌊(n + √(5*n²)) / 2⌋ = ⌊(n + ⌊√(5*n²)⌋) / 2⌋ */
    seq_t s2 = sqrt_floor((seq_t)5 * nn * nn);
    seq_t res = nn + s1 + (nn + s2) / (seq_t)2;
    return res;
}
