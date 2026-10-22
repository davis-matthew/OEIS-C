/*
 * A007401  Add n-1 to n-th term of 'n appears n times' sequence (A002024).
 * https://oeis.org/A007401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((t+2)^2 + i - j)/2 - 1, where t =
 *            floor((-1+sqrt(8*n-7))/2), i = n-t*(t+1)/2, j = (t*t+3*t+4)/2-n
 * Range:     u8 n <= 234, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000037_isqrt.h"

seq_t a(idx_t n) {
    uint64_t discriminant = 8 * n - 7;
    uint64_t sqrt_disc = a000037_isqrt(discriminant);
    idx_t t = (sqrt_disc - 1) / 2;
    
    seq_t i = (seq_t)n - (seq_t)t * ((seq_t)t + 1) / 2;
    seq_t j = ((seq_t)t * (seq_t)t + 3 * (seq_t)t + 4) / 2 - (seq_t)n;
    
    seq_t result = (((seq_t)t + 2) * ((seq_t)t + 2) + i - j) / 2 - 1;
    
    return result;
}
