/*
 * A002217  Starting with n, repeatedly calculate the sum of prime factors
 *          (with repetition) of the previous term, until reaching 0 or a
 *          fixed point: a(n) is the number of terms in the resulting
 *          sequence.
 * https://oeis.org/A002217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n, repeatedly compute the sum of prime factors (with
 *            repetition) until reaching 0 or a fixed point; return the count
 *            of terms in the resulting sequence
 * Range:     u8 n <= 2409, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_sum_of_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_sum_of_prime_factors.h"

seq_t a(idx_t n) {
    seq_t current = (seq_t)n;
    seq_t count = 1;
    
    while (1) {
        if (current == 0) {
            break;
        }
        
        seq_t sum_of_prime_factors = compute_sum_of_prime_factors(current);
        
        if (sum_of_prime_factors == current) {
            break;
        }
        
        current = sum_of_prime_factors;
        count++;
    }
    
    return count;
}
