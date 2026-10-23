/*
 * A117703  Palindromic primes in base 9 (written in base 9).
 * https://oeis.org/A117703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate base‑9 palindromes in numerical order,
   test primality of their base‑10 value, and
   return the base‑9 digits interpreted as a decimal integer. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;

    for (idx_t len = 1; /* no upper bound */ ; ++len) {
        idx_t left_len = (len + 1) / 2;

        /* first digit of a palindrome cannot be 0, so start = 9^(left_len-1) */
        idx_t start = 1;
        for (idx_t i = 1; i < left_len; ++i) start *= 9;
        /* last left value: 9^left_len - 1 */
        idx_t end = start * 9 - 1;

        for (idx_t left = start; left <= end; ++left) {
            /* digits of the left part (most significant first) */
            uint8_t digitsL[64];
            idx_t tmp = left;
            for (int i = (int)left_len - 1; i >= 0; --i) {
                digitsL[i] = (uint8_t)(tmp % 9);
                tmp /= 9;
            }

            /* build the full palindrome digits */
            uint8_t digits[128];
            for (idx_t i = 0; i < left_len; ++i)
                digits[i] = digitsL[i];

            idx_t pos = left_len;
            if (len % 2 == 0) { /* even length: mirror all left digits */
                for (idx_t j = 0; j < left_len; ++j)
                    digits[pos++] = digitsL[left_len - 1 - j];
            } else {            /* odd length: mirror all except the middle one */
                for (idx_t j = 0; j < left_len - 1; ++j)
                    digits[pos++] = digitsL[left_len - 2 - j];
            }

            /* evaluate the value in base 10 (for primality) and
               the value of the decimal digit string */
            seq_t valPrime  = 0;
            seq_t valDigits = 0;
            for (idx_t i = 0; i < len; ++i) {
                valPrime  = valPrime  * 9 + digits[i];
                valDigits = valDigits * 10 + digits[i];
            }

            if (is_prime_big(valPrime)) {
                ++count;
                if (count == n)
                    return valDigits;
            }
        }
    }
}
