/*
 * A007430  Inverse Moebius transform applied thrice to natural numbers.
 * https://oeis.org/A007430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) = Sum_{d|n} sigma(d)*tau(n/d)
 * Range:     u8 n <= 35, u16 n <= 2519, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tau
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/tau.h"

static seq_t sigma(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; i++) {
        if (n % i == 0) {
            sum += (seq_t)i;
            if (i != n / i) {
                sum += (seq_t)(n / i);
            }
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t d = 1; d <= n / d; d++) {
        if (n % d == 0) {
            result += sigma(d) * tau(n / d);
            
            if (d != n / d) {
                idx_t d2 = n / d;
                result += sigma(d2) * tau(n / d2);
            }
        }
    }
    
    return result;
}
