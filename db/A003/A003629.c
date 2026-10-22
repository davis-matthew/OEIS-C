/*
 * A003629  Primes p == +- 3 (mod 8), or, primes p such that 2 is not a square
 *          mod p.
 * https://oeis.org/A003629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime p such that p ≡ ±3 (mod 8)
 * Range:     u8 n <= 29, u16 n <= 3278, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 3;
    
    while (true) {
        if (is_prime((uint64_t)candidate)) {
            seq_t mod8 = candidate % 8;
            if (mod8 == 3 || mod8 == 5) {
                count++;
                if (count == n) {
                    return candidate;
                }
            }
        }
        candidate += 2;
    }
}
