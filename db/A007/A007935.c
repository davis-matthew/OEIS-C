/*
 * A007935  Composite numbers such that some permutation of digits is a prime.
 * https://oeis.org/A007935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) searches for the n-th composite number such that some
 *            permutation of its digits is prime
 * Range:     u8 n <= 64, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   is_prime, swap_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/swap_int.h"

static bool check_perms(int *digits, int len, int start) {
    if (start == len - 1) {
        uint64_t num = 0;
        for (int i = 0; i < len; i++) {
            num = num * 10 + digits[i];
        }
        return is_prime(num);
    }
    
    for (int i = start; i < len; i++) {
        swap_int(&digits[start], &digits[i]);
        if (check_perms(digits, len, start + 1)) {
            return true;
        }
        swap_int(&digits[start], &digits[i]);
    }
    
    return false;
}

static bool has_prime_perm(seq_t n) {
    int digits[20];
    int len = 0;
    seq_t temp = n;
    
    while (temp > 0) {
        digits[len++] = (int)(temp % 10);
        temp /= 10;
    }
    
    for (int i = 0; i < len / 2; i++) {
        int tmp = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = tmp;
    }
    
    return check_perms(digits, len, 0);
}

static bool is_composite(seq_t n) {
    if (n <= 1) return false;
    return !is_prime((uint64_t)n);
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t current = 2;
    
    while (1) {
        if (is_composite(current) && has_prime_perm(current)) {
            count++;
            if (count == n) {
                return current;
            }
        }
        current++;
    }
}
