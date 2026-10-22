/*
 * A006954  Denominators of Bernoulli numbers B_0, B_1, B_2, B_4, B_6, ...
 * https://oeis.org/A006954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   von Staudt-Clausen theorem for Bernoulli number denominators *
 *            a(0) = 1 (B_0 = 1) * a(1) = 2 (B_1 = -1/2) * a(n) for n >= 2 =
 *            product of primes p where (p-1) divides 2(n-1)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"


seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    if (n == 1) {
        return 2;
    }
    
    idx_t b = 2 * (n - 1);
    seq_t result = 1;
    
    for (idx_t d = 1; d * d <= b; d++) {
        if (b % d == 0) {
            if (is_prime(d + 1)) {
                result *= (seq_t)(d + 1);
            }
            idx_t d2 = b / d;
            if (d != d2 && is_prime(d2 + 1)) {
                result *= (seq_t)(d2 + 1);
            }
        }
    }
    
    return result;
}
