/*
 * A007965  a(n) = n^prime(n) - prime(n)^n.
 * https://oeis.org/A007965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n^prime(n) - prime(n)^n
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 8, i128 n <= 11,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 96 (2026-11-03)
 * Helpers:   ipow_idx, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
        
    // Find the n-th prime
    idx_t count = 0;
    uint64_t candidate = 2;
    uint64_t prime_n = 0;
    
    while (count < n) {
        if (is_prime_u64(candidate)) {
            count++;
            if (count == n) {
                prime_n = candidate;
                break;
            }
        }
        candidate++;
    }
    
    // Compute n^prime(n) - prime(n)^n
    seq_t term1 = ipow_idx((seq_t)n, prime_n);
    seq_t term2 = ipow_idx((seq_t)prime_n, n);
    
    return term1 - term2;
}
