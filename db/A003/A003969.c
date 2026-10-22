/*
 * A003969  Inverse Möbius transform of A003959.
 * https://oeis.org/A003969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   Multiplicative with a(p^e) = ((p+1)^(e+1)-1)/p
 * Range:     u8 n <= 31, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"


seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t temp = n;
    
    // Trial division to find prime factors
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            // Found a prime factor p
            idx_t e = 0;
            while (temp % p == 0) {
                e++;
                temp /= p;
            }
            // Compute contribution a(p^e) = ((p+1)^(e+1) - 1) / p
            seq_t p_plus_1 = (seq_t)(p + 1);
            seq_t power = a000272_ipow(p_plus_1, (idx_t)(e + 1));
            seq_t numerator = power - 1;
            seq_t contribution = numerator / (seq_t)p;
            result *= contribution;
        }
    }
    
    // If temp > 1, it's a prime factor with exponent 1
    if (temp > 1) {
        result *= ((seq_t)temp + 2);
    }
    
    return result;
}
