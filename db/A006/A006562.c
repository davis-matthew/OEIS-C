/*
 * A006562  Balanced primes (of order one): primes which are the average of
 *          the previous prime and the following prime.
 * https://oeis.org/A006562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Balanced primes: primes p where 2*p = p_prev + p_next
 * Range:     u8 n <= 5, u16 n <= 308, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9873 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    uint64_t p_prev = 2;
    uint64_t p_curr = 3;
    uint64_t p_next = 5;
    idx_t count = 0;
    
    while (true) {
        // Check if p_curr is balanced
        if (2 * p_curr == p_prev + p_next) {
            count++;
            if (count == n) {
                return (seq_t)p_curr;
            }
        }
        
        // Move to next prime
        p_prev = p_curr;
        p_curr = p_next;
        p_next += 2;
        while (!is_prime_u64(p_next)) {
            p_next += 2;
        }
    }
}
