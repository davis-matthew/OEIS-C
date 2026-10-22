/*
 * A004641  Fixed under 0 -> 10, 1 -> 100.
 * https://oeis.org/A004641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = floor(n*(sqrt(2) - 1) + sqrt(1/2)) - floor((n -
 *            1)*(sqrt(2) - 1) + sqrt(1/2))
 * Range:     u8 n <= 8119, u16 n <= 8119, u32 n <= 8119, u64 n <= 8119,
 *            u128 n <= 8119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>


seq_t a(idx_t n) {
    long double sqrt2_minus_1 = 0.41421356237309504880168872420969807856967187537694807317667973799L;
    long double sqrt_half = 0.70710678118654752440084436210484903928483593768847403658833986900L;
    
    long double val_n = (long double)n * sqrt2_minus_1 + sqrt_half;
    long double val_n_minus_1 = ((long double)n - 1.0L) * sqrt2_minus_1 + sqrt_half;
    
    long long floor_n = floorl(val_n);
    long long floor_n_minus_1 = floorl(val_n_minus_1);
    
    return (seq_t)(floor_n - floor_n_minus_1);
}
