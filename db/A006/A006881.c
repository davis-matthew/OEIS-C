/*
 * A006881  Squarefree semiprimes: Numbers that are the product of two
 *          distinct primes.
 * https://oeis.org/A006881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) is the n-th squarefree semiprime (product of exactly two
 *            distinct primes), computed by trial division
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree_semiprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree_semiprime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 6;
    
    while (true) {
        if (is_squarefree_semiprime(candidate)) {
            count++;
            if (count == n) return (seq_t)candidate;
        }
        candidate++;
    }
}
