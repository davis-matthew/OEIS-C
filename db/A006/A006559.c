/*
 * A006559  Short period primes: the decimal expansion of 1/p has period less
 *          than p-1, but greater than zero.
 * https://oeis.org/A006559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, base
 * Formula:   a(n) = n-th prime p where ord_p(10) in (0, p-1)
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, multiplicative_order
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/multiplicative_order.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p = 3;
    
    while (true) {
        if (is_prime(p)) {
            uint64_t order = multiplicative_order(10, p);
            if (order > 0 && order < p - 1) {
                count++;
                if (count == n) {
                    return (seq_t)p;
                }
            }
        }
        p += 2;
    }
}
