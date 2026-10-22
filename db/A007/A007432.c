/*
 * A007432  Moebius transform applied thrice to natural numbers.
 * https://oeis.org/A007432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice, mult
 * Formula:   Multiplicative with a(p^e) = Sum_{k=0..3} (-1)^k
 *            C(3,k)*p^(e-k)[e>=k]
 * Range:     i8 n <= 130, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   compute_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/compute_prime_power.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t temp = n;
    
    // Factor out 2
    idx_t e = 0;
    while (temp % 2 == 0) {
        e++;
        temp /= 2;
    }
    if (e > 0) {
        result *= compute_prime_power(2, e);
    }
    
    // Factor out odd primes
    for (idx_t p = 3; p * p <= temp; p += 2) {
        e = 0;
        while (temp % p == 0) {
            e++;
            temp /= p;
        }
        if (e > 0) {
            result *= compute_prime_power((seq_t)p, e);
        }
    }
    
    // Remaining factor is prime
    if (temp > 1) {
        result *= compute_prime_power((seq_t)temp, 1);
    }
    
    return result;
}
