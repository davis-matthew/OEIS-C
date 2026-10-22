/*
 * A007693  Primes p such that 6*p + 1 is also prime.
 * https://oeis.org/A007693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Finding primes p such that 6*p + 1 is also prime
 * Range:     u8 n <= 23, u16 n <= 1475, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p = 2;
    
    while (true) {
        if (is_prime(p)) {
            uint64_t candidate = 6 * p + 1;
            if (is_prime(candidate)) {
                count++;
                if (count == n) {
                    return (seq_t)p;
                }
            }
        }
        if (p == 2) {
            p = 3;
        } else {
            p += 2;
        }
    }
}
