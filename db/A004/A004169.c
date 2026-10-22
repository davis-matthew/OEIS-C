/*
 * A004169  Values of m for which a regular polygon with m sides cannot be
 *          constructed with ruler and compass.
 * https://oeis.org/A004169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Numbers m for which φ(m) is not a power of 2
 * Range:     u8 n <= 219, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_power_of_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_power_of_2.h"

static seq_t compute_phi(seq_t m) {
    seq_t result = m;
    seq_t n = m;
    
    if (n % 2 == 0) {
        while (n % 2 == 0) {
            n /= 2;
        }
        result /= 2;
    }
    
    for (seq_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            while (n % i == 0) {
                n /= i;
            }
            result = result / i * (i - 1);
        }
    }
    
    if (n > 1) {
        result = result / n * (n - 1);
    }
    
    return result;
}

seq_t a(idx_t n) {
    seq_t target = (seq_t)n;
    seq_t count = 0;
    seq_t m = 1;
    
    while (1) {
        seq_t phi_m = compute_phi(m);
        if (!is_power_of_2(phi_m)) {
            count++;
            if (count == target) {
                return m;
            }
        }
        m++;
    }
}
