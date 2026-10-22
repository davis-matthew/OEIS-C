/*
 * A006338  An "eta-sequence": floor((n+1)*sqrt(2) + 1/2) - floor(n*sqrt(2) +
 *          1/2).
 * https://oeis.org/A006338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor((n+1)*sqrt(2) + 1/2) - floor(n*sqrt(2) + 1/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    long double sqrt2 = sqrtl(2.0L);
    long double val_n = (long double)n * sqrt2;
    long double val_n1 = (long double)(n + 1) * sqrt2;
    
    long long floor_n = (long long)floorl(val_n + 0.5L);
    long long floor_n1 = (long long)floorl(val_n1 + 0.5L);
    
    return (seq_t)(floor_n1 - floor_n);
}
