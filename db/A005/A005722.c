/*
 * A005722  a(n) = (prime(n) - 1)^2.
 * https://oeis.org/A005722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n) - 1)^2
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t candidate = 2;
    
    while (true) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) {
                seq_t p_minus_1 = (seq_t)(candidate - 1);
                return p_minus_1 * p_minus_1;
            }
        }
        candidate++;
    }
}
