/*
 * A002640  Numbers k such that (k^2 + k + 1)/3 is prime.
 * https://oeis.org/A002640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k such that (k^2 + k + 1)/3 is prime
 * Range:     u8 n <= 4, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 1;
    
    while (1) {
        seq_t k2 = k * k;
        seq_t numerator = k2 + k + 1;
        seq_t p = numerator / 3;
        
        if (is_prime_seq(p)) {
            count++;
            if (count == n) return k;
        }
        
        k += 3;
    }
}
