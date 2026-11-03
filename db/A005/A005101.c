/*
 * A005101  Abundant numbers (sum of divisors of m exceeds 2m).
 * https://oeis.org/A005101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 4807, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9840 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n-th abundant number by iteration; m is abundant if sigma(m) > 2m */

#include <stdbool.h>

static seq_t sum_of_divisors(seq_t m) {
    seq_t sum = 1 + m;
    
    for (seq_t i = 2; i * i <= m; i++) {
        if (m % i == 0) {
            sum += i;
            if (i != m / i) {
                sum += m / i;
            }
        }
    }
    return sum;
}

static bool is_abundant(seq_t m) {
    seq_t sigma = sum_of_divisors(m);
    return sigma > 2 * m;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 11;
    
    while (count < n) {
        m++;
        if (is_abundant(m)) {
            count++;
        }
    }
    
    return m;
}
