/*
 * A083393  Palindromes such that the sum of the digits is prime.
 * https://oeis.org/A083393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 194, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, make_pal, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth palindrome whose digit sum is a prime, in increasing order. */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/is_prime.h"
#include "../utils/make_pal.h"

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
