/*
 * A005381  Numbers k such that k and k-1 are composite.
 * https://oeis.org/A005381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) finds the nth number k such that both k and k-1 are
 *            composite
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 8;
    
    while (count < n) {
        k++;
        if (!is_prime((uint64_t)k) && !is_prime((uint64_t)(k - 1))) {
            count++;
        }
    }
    
    return k;
}
