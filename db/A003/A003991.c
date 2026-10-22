/*
 * A003991  Multiplication table read by antidiagonals: T(i,j) = i*j, i>=1,
 *          j>=1.
 * https://oeis.org/A003991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, nice, easy, look
 * Formula:   a(n) = pos*(row+1-pos), where row = floor((1+sqrt(8n-7))/2), pos
 *            = n - row*(row-1)/2
 * Range:     u8 n <= 480, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000037_isqrt.h"

seq_t a(idx_t n) {
    uint64_t sqrt_val = a000037_isqrt(8*n - 7);
    idx_t row = (sqrt_val + 1) / 2;
    idx_t pos = n - row*(row-1)/2;
    
    return (seq_t)pos * (seq_t)(row + 1 - pos);
}
