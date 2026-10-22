/*
 * A007634  Numbers k such that k^2 + k + 41 is composite.
 * https://oeis.org/A007634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) finds the n-th positive integer k such that k^2 + k + 41 is
 *            composite
 * Range:     u8 none, u16 n <= 166, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_val.h"

seq_t a(idx_t n) {
    seq_t k = 0;
    idx_t count = 0;
    
    while (true) {
        seq_t val = k * k + k + 41;
        if (!is_prime_val(val)) {
            count++;
            if (count == n) {
                return k;
            }
        }
        k++;
    }
}
