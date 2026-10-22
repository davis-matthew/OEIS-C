/*
 * A005097  (Odd primes - 1)/2.
 * https://oeis.org/A005097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n+1) - 1) / 2
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    
    for (uint64_t candidate = 2; ; candidate++) {
        if (is_prime(candidate)) {
            count++;
            if (count == n + 1) {
                return (seq_t)((candidate - 1) / 2);
            }
        }
    }
}
