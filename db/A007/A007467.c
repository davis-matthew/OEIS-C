/*
 * A007467  Product of next n primes.
 * https://oeis.org/A007467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product of n consecutive primes starting at prime(1 +
 *            n*(n-1)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"


seq_t a(idx_t n) {
    idx_t start_idx = 1 + (idx_t)(((n - 1) * n) / 2);
    idx_t end_idx = start_idx + n;
    
    seq_t result = 1;
    idx_t prime_idx = 0;
    uint64_t candidate = 2;
    
    while (prime_idx < end_idx) {
        if (is_prime(candidate)) {
            prime_idx++;
            if (prime_idx >= start_idx && prime_idx < end_idx) {
                result *= (seq_t)candidate;
            }
        }
        if (candidate == 2) {
            candidate = 3;
        } else {
            candidate += 2;
        }
    }
    
    return result;
}
