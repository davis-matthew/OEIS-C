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
 * Helpers:   f_value, prime_power_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/f_value.h"
#include "../utils/prime_power_exponent.h"

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
