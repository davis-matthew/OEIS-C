/*
 * A007933  Some permutation of digits is a prime.
 * https://oeis.org/A007933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search -- a(n) is the n-th positive integer that has a digit
 *            permutation (leading zeros allowed) that is prime
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static bool check_perm(const char *digits, int len) {
    uint64_t num = 0;
    for (int i = 0; i < len; i++) {
        num = num * 10 + (uint64_t)(digits[i] - '0');
    }
    return is_prime(num);
}

static bool gen_perm(char *digits, int len, int pos) {
    if (pos == len) {
        return check_perm(digits, len);
    }

    for (int i = pos; i < len; i++) {
        bool tried = false;
        for (int j = pos; j < i; j++) {
            if (digits[j] == digits[i]) {
                tried = true;
                break;
            }
        }
        if (tried) continue;

        char tmp = digits[pos];
        digits[pos] = digits[i];
        digits[i] = tmp;

        bool found = gen_perm(digits, len, pos + 1);

        tmp = digits[pos];
        digits[pos] = digits[i];
        digits[i] = tmp;

        if (found) return true;
    }

    return false;
}

static bool has_prime_perm(uint64_t num) {
    if (num == 0) return false;

    char digits[21];
    int len = 0;
    uint64_t temp = num;
    while (temp > 0) {
        digits[len++] = (char)('0' + (int)(temp % 10));
        temp /= 10;
    }

    return gen_perm(digits, len, 0);
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t m = 0;

    while (count < n) {
        m++;
        if (has_prime_perm(m)) {
            count++;
        }
    }

    return (seq_t)m;
}
