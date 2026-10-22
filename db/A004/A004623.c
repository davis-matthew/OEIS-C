/*
 * A004623  Divisible only by primes congruent to 5 mod 7.
 * https://oeis.org/A004623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Successive numbers whose prime factors are all congruent to 5
 *            mod 7, found by trial factorization
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_valid(seq_t num) {
    if (num == 1) return true;
    
    seq_t temp = num;
    
    for (seq_t i = 2; i <= num / i; i++) {
        if (temp % i == 0) {
            if (i % 7 != 5) return false;
            while (temp % i == 0) {
                temp /= i;
            }
        }
    }
    
    if (temp > 1) {
        if (temp % 7 != 5) return false;
    }
    
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t num = 0;
    
    while (count < n) {
        num++;
        if (is_valid(num)) {
            count++;
        }
    }
    
    return num;
}
