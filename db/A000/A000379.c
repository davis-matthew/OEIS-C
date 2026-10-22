/*
 * A000379  Numbers where total number of 1-bits in the exponents of their
 *          prime factorization is even; a 2-way classification of integers:
 *          complement of A000028.
 * https://oeis.org/A000379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   in_a000379, popcount_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterates through positive integers and returns those with even total 1-bits in prime exponents */

#include <stdbool.h>
#include "../utils/in_a000379.h"
#include "../utils/popcount_u64.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t m = 0;
    
    while (count < (seq_t)n) {
        m++;
        if (in_a000379((uint64_t)m)) {
            count++;
        }
    }
    
    return m;
}
