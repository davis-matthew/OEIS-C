/*
 * A004955  a(n) = round(n*phi^20), where phi is the golden ratio, A001622.
 * https://oeis.org/A004955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n * phi^20) where phi = golden ratio
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
        
    long double phi = (1.0L + sqrtl(5.0L)) / 2.0L;
    long double phi20 = 6765.0L * phi + 4181.0L;
    long double result = (long double)n * phi20;
    
    return (seq_t)(long long)roundl(result);
}
