/*
 * A003684  Number of n-digit reversible primes (or emirps) with distinct
 *          digits.
 * https://oeis.org/A003684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base, easy, nice
 * Formula:   Count n-digit numbers with distinct digits that are primes and
 *            whose reverse is also prime
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   reverse_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/reverse_number.h"

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

static seq_t count_emirps(uint64_t current, int remaining_digits, uint16_t used_digits) {
    if (remaining_digits == 0) {
        if (is_prime(current) && is_prime(reverse_number(current))) return 1;
        return 0;
    }
    
    seq_t count = 0;
    for (int digit = 0; digit < 10; digit++) {
        if (!(used_digits & (1 << digit))) {
            count += count_emirps(current * 10 + digit, remaining_digits - 1, used_digits | (1 << digit));
        }
    }
    return count;
}

static seq_t compute_a(int num_digits) {
    if (num_digits == 1) return 4;
    
    seq_t count = 0;
    for (int first_digit = 1; first_digit <= 9; first_digit++) {
        count += count_emirps(first_digit, num_digits - 1, 1 << first_digit);
    }
    
    return count;
}

seq_t a(idx_t n) {
    static seq_t cache[10] = {0};
    static bool computed[10] = {false};
    
    if (n < 1 || n > 9) return 0;
    
    int index = (int)(n - 1);
    if (!computed[index]) {
        cache[index] = compute_a((int)n);
        computed[index] = true;
    }
    
    return cache[index];
}
