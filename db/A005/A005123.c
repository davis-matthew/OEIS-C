/*
 * A005123  Numbers k such that 8k + 1 is prime.
 * https://oeis.org/A005123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k, where 8k+1 is the n-th prime of form 8m+1
 * Range:     u8 n <= 69, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 0;
    
    while (1) {
        uint64_t candidate = 8 * (uint64_t)k + 1;
        if (is_prime(candidate)) {
            count++;
            if (count == n) {
                return k;
            }
        }
        k++;
    }
}
