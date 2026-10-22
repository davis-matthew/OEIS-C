/*
 * A007640  Numbers k such that 2*k^2 - 2*k + 19 is composite.
 * https://oeis.org/A007640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Find the n-th number k such that 2*k^2 - 2*k + 19 is composite
 * Range:     u8 n <= 127, u16 n <= 3400, u32 n <= 3400, u64 n <= 3400,
 *            u128 n <= 3400, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"


seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 0;
    
    while (count < n) {
        k++;
        uint64_t value = 2ULL * (uint64_t)k * (uint64_t)k - 2ULL * (uint64_t)k + 19;
        
        if (!is_prime(value)) {
            count++;
        }
    }
    
    return k;
}
