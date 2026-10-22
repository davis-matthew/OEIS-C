/*
 * A003971  Inverse Möbius transform of A003960.
 * https://oeis.org/A003971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   Multiplicative with a(p^e) = e+1 if p = 2; (((p+1)/2)^(e+1) -
 *            1)/((p-1)/2) if p > 2
 * Range:     u8 n <= 36, u16 n <= 1657, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_odd_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_odd_prime_power.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t temp = n;
    
    // Handle factor 2
    if (temp % 2 == 0) {
        idx_t e = 0;
        while (temp % 2 == 0) {
            e++;
            temp /= 2;
        }
        result *= (seq_t)(e + 1);
    }
    
    // Handle odd prime factors
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                e++;
                temp /= p;
            }
            result *= compute_odd_prime_power((seq_t)p, e);
        }
    }
    
    // Handle remaining prime factor if any
    if (temp > 1) {
        result *= compute_odd_prime_power((seq_t)temp, 1);
    }
    
    return result;
}
