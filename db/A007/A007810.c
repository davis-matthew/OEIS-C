/*
 * A007810  Largest prime with n distinct decimal digits.
 * https://oeis.org/A007810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base, full
 * Formula:   search for the largest prime with exactly n distinct decimal
 *            digits
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

static int count_distinct_digits(uint64_t num) {
    int digits_seen[10] = {0};
    int count = 0;
    
    if (num == 0) return 1;
    
    while (num > 0) {
        int digit = (int)(num % 10);
        if (!digits_seen[digit]) {
            digits_seen[digit] = 1;
            count++;
        }
        num /= 10;
    }
    
    return count;
}

seq_t a(idx_t n) {
    if (n > 10) {
        return 0;
    }
    
    uint64_t start = 0;
    for (uint64_t i = 0; i < n; i++) {
        start = start * 10 + (9 - i);
    }
    
    for (uint64_t candidate = start; candidate > 0; candidate--) {
        if (count_distinct_digits(candidate) == (int)n && is_prime(candidate)) {
            return (seq_t)candidate;
        }
    }
    
    return 0;
}
