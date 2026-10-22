/*
 * A005101  Abundant numbers (sum of divisors of m exceeds 2m).
 * https://oeis.org/A005101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 4807, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_abundant, sum_of_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n-th abundant number by iteration; m is abundant if sigma(m) > 2m */

#include <stdbool.h>
#include "../utils/is_abundant.h"
#include "../utils/sum_of_divisors.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 11;
    
    while (count < n) {
        m++;
        if (is_abundant(m)) {
            count++;
        }
    }
    
    return m;
}
