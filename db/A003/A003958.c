/*
 * A003958  If n = Product p(k)^e(k) then a(n) = Product (p(k)-1)^e(k).
 * https://oeis.org/A003958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, nice
 * Formula:   Multiplicative with a(p^e) = (p-1)^e
 * Range:     u8 n <= 256, u16 n <= 9035, u32 n <= 9035, u64 n <= 9035,
 *            u128 n <= 9035, big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t temp = n;
    
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            seq_t prime_minus_1 = (seq_t)(p - 1);
            idx_t exponent = 0;
            
            while (temp % p == 0) {
                exponent++;
                temp /= p;
            }
            
            result *= a000248_pow(prime_minus_1, exponent);
        }
    }
    
    if (temp > 1) {
        result *= (seq_t)(temp - 1);
    }
    
    return result;
}
