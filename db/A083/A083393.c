/*
 * A083393  Palindromes such that the sum of the digits is prime.
 * https://oeis.org/A083393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 194, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 8625 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth palindrome whose digit sum is a prime, in increasing order. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static seq_t make_pal(seq_t prefix, bool odd) {
    seq_t pal = prefix;
    seq_t temp = odd ? prefix / 10 : prefix;
    while (temp > 0) {
        pal = pal * 10 + (temp % 10);
        temp = temp / 10;
    }
    return pal;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    /* Enumerate palindromes by increasing length. */
    for (unsigned len = 1; ; ++len) {
        unsigned half = (len + 1) / 2;           /* number of digits in the left half */
        seq_t start = pow10(half - 1);           /* smallest half of this length */
        seq_t end   = pow10(half) - 1;           /* largest half of this length */

        for (seq_t prefix = start; prefix <= end; ++prefix) {
            seq_t pal = make_pal(prefix, len % 2 == 1);

            unsigned long long sum = 0;
            seq_t tmp = pal;
            while (tmp > 0) {
                sum += (unsigned long long)(tmp % 10);
                tmp /= 10;
            }

            if (is_prime((idx_t)sum)) {
                ++count;
                if (count == n) return pal;
            }
        }
    }
}
