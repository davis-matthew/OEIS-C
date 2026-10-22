/*
 * A002035  Numbers that contain primes to odd powers only.
 * https://oeis.org/A002035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   n-th number whose prime factorization contains only odd
 *            exponents
 * Range:     u8 n <= 182, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_all_odd_exponents
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_all_odd_exponents.h"

seq_t a(idx_t n) {
    seq_t target = (seq_t)n;
    seq_t count = 0;
    seq_t num = 1;
    
    while (count < target) {
        num++;
        if (has_all_odd_exponents(num)) {
            count++;
        }
    }
    
    return num;
}
