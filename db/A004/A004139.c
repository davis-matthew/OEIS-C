/*
 * A004139  Odd primes excluding 5.
 * https://oeis.org/A004139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th odd prime excluding 5
 * Range:     u8 n <= 52, u16 n <= 6540, u32 n <= 10000, u64 n <= 10000,
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
    
    while (1) {
        if (candidate != 5 && is_prime((uint64_t)candidate)) {
            count++;
            if (count == n) {
                return candidate;
            }
        }
        candidate += 2;
    }
}
