/*
 * A046395  Palindromes that are the product of 5 distinct primes.
 * https://oeis.org/A046395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 500, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for palindromes that are the product of 5 distinct primes. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* powers of 10 up to 10^18 (fits in uint64_t). */
static const uint64_t pow10[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL, 1000000000000000000ULL,
    10000000000000000000ULL
};

/* reverse the decimal digits of x */
static uint64_t rev_digits(uint64_t x)
{
    uint64_t r = 0;
    while (x) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

/* construct the palindrome from a prefix of length ⌈len/2⌉ */
static uint64_t make_palindrome(uint64_t prefix, int len)
{
    int half = len >> 1;                /* floor(len/2) */
    if ((len & 1) == 0) {               /* even length */
        return prefix * pow10[half] + rev_digits(prefix);
    } else {                            /* odd length */
        return prefix * pow10[half] + rev_digits(prefix / 10);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset starts at 1 */

    idx_t found = 0;                    /* number of terms found */
    int len = 1;                        /* current palindrome length */

    while (found < n) {
        /* number of digits in the prefix */
        int h = (len + 1) >> 1;                /* ceil(len/2) */
        uint64_t start = (h == 1) ? 1ULL : pow10[h - 1];
        uint64_t end   = pow10[h] - 1ULL;

        for (uint64_t prefix = start; prefix <= end; ++prefix) {
            uint64_t pal = make_palindrome(prefix, len);

            /* factor the candidate pal */
            uint64_t primes[20], exponents[20];
            size_t prime_cnt = 0;
            factor_u64(pal, primes, exponents, &prime_cnt);

            if (prime_cnt == 5) {
                bool all_single = true;
                for (size_t i = 0; i < prime_cnt; ++i) {
                    if (exponents[i] != 1) {
                        all_single = false;
                        break;
                    }
                }
                if (all_single) {
                    ++found;
                    if (found == n) {
                        return (seq_t)pal;
                    }
                }
            }
        }
        /* move to next palindrome length */
        ++len;
    }

    /* should never reach here */
    return (seq_t)0;
}
