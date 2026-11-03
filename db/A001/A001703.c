/*
 * A001703  Decimal concatenation of n, n+1, and n+2.
 * https://oeis.org/A001703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of n, n+1, n+2 in decimal
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 997, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 53 terms through n = 806 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/num_digits.h"

static seq_t power_of_10(int k) {
    seq_t result = 1;
    for (int i = 0; i < k; i++) {
        result *= 10;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t num_n = (seq_t)n;
    seq_t num_n1 = num_n + 1;
    seq_t num_n2 = num_n + 2;
    
    int digits_n = num_digits(num_n);
    int digits_n1 = num_digits(num_n1);
    int digits_n2 = num_digits(num_n2);
    
    if (digits_n == digits_n1 && digits_n1 == digits_n2) {
        int d = digits_n;
        seq_t pow10d = power_of_10(d);
        seq_t result = (pow10d * pow10d + pow10d + 1) * num_n + (pow10d + 2);
        return result;
    } else {
        seq_t pow10_n1n2 = power_of_10(digits_n1 + digits_n2);
        seq_t pow10_n2 = power_of_10(digits_n2);
        seq_t result = num_n * pow10_n1n2 + num_n1 * pow10_n2 + num_n2;
        return result;
    }
}
