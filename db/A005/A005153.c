/*
 * A005153  Practical numbers: positive integers m such that every k <=
 *          sigma(m) is a sum of distinct divisors of m. Also called
 *          panarithmic numbers.
 * https://oeis.org/A005153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 7958, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_practical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_practical.h"

/* Search: Practical numbers by Stewart's characterization theorem */
seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 0;
    
    while (count < n) {
        m++;
        if (is_practical(m)) {
            count++;
        }
    }
    
    return m;
}
