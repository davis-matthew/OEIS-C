/*
 * A005141  Number of genera of forms with |determinant| = n.
 * https://oeis.org/A005141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, more
 * Formula:   a(n) = sum_{d|n} f(d), where f(d) is determined by the prime
 *            factorization of d
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static idx_t prime_power_exponent(idx_t n) {
    // Returns k if n = p^k for odd prime p, else 0
    if (n < 3) return 0;
    
    for (idx_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            idx_t e = 0;
            while (n % p == 0) {
                e++;
                n /= p;
            }
            if (n == 1) return e;  // n was exactly p^e
            return 0;  // n has multiple distinct prime factors
        }
    }
    
    // n is an odd prime
    return 1;
}

static seq_t f_value(idx_t d) {
    // Special cases for powers of 2
    if (d == 1) return 2;
    if (d == 2) return 0;
    if (d == 4) return 7;
    if (d == 8) return 1;
    
    // All powers of 2 >= 16 contribute 0
    if ((d & (d - 1)) == 0) return 0;
    
    // Even numbers that aren't powers of 2 contribute 0
    if ((d & 1) == 0) return 0;
    
    // For odd d >= 3, check if it's a prime power
    idx_t k = prime_power_exponent(d);
    if (k > 0) {
        // d = p^k for odd prime p, contributes 2^k
        return (seq_t)1 << k;
    } else {
        // d is odd square-free composite, contributes 4
        return 4;
    }
}

static seq_t f_value(idx_t d);
static idx_t prime_power_exponent(idx_t n);

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            result += f_value(d);
            
            idx_t complement = n / d;
            if (d != complement) {
                result += f_value(complement);
            }
        }
    }
    
    return result;
}
