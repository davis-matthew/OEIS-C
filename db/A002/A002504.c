/*
 * A002504  Numbers x such that 1 + 3*x*(x-1) is a ("cuban") prime (cf.
 *          A002407).
 * https://oeis.org/A002504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = nth positive integer x such that 1 + 3*x*(x-1) is prime
 * Range:     u8 n <= 5, u16 n <= 53, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 949 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t x = 1;
    
    while (1) {
        seq_t p = (seq_t)1 + (seq_t)3 * x * (x - (seq_t)1);
        
        if (is_prime_u64((uint64_t)p)) {
            count++;
            if (count == n) {
                return x;
            }
        }
        x++;
    }
}
