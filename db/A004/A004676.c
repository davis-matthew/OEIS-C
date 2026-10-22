/*
 * A004676  Primes written in base 2.
 * https://oeis.org/A004676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A007088(A000040(n))
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 172, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"


seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 2;
    
    while (count < n) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) {
                break;
            }
        }
        candidate++;
    }
    
    uint64_t prime = candidate;
    
    seq_t result = 0;
    seq_t power = 1;
    
    while (prime > 0) {
        seq_t bit = (seq_t)(prime & 1);
        result += bit * power;
        power *= 10;
        prime >>= 1;
    }
    
    return result;
}
