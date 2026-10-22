/*
 * A006753  Smith (or joke) numbers: composite numbers k such that sum of
 *          digits of k = sum of digits of prime factors of k (counted with
 *          multiplicity).
 * https://oeis.org/A006753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   Find the nth Smith number by iterating through candidates and
 *            checking if sum of digits equals sum of digits of prime factors
 *            with multiplicity
 * Range:     u8 n <= 9, u16 n <= 2112, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static uint64_t digit_sum_u64(uint64_t n) {
    uint64_t sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

static bool is_smith_number(uint64_t n) {
    uint64_t digit_sum_n = digit_sum_u64(n);
    uint64_t factor_digit_sum = 0;
    uint64_t temp = n;
    int factor_count = 0;
    
    // Factor out 2s
    while (temp % 2 == 0) {
        factor_digit_sum += 2;
        factor_count++;
        temp /= 2;
    }
    
    // Try odd factors
    for (uint64_t i = 3; i * i <= temp; i += 2) {
        while (temp % i == 0) {
            factor_digit_sum += digit_sum_u64(i);
            factor_count++;
            temp /= i;
        }
    }
    
    // Remaining temp is a prime factor if > 1
    if (temp > 1) {
        factor_digit_sum += digit_sum_u64(temp);
        factor_count++;
    }
    
    // Must be composite and satisfy the digit sum condition
    return factor_count >= 2 && digit_sum_n == factor_digit_sum;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 4;
    
    while (true) {
        if (is_smith_number(candidate)) {
            count++;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        candidate++;
    }
}
