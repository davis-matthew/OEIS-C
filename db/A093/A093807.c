/*
 * A093807  Palindromic numbers with property that sum of digits is prime and
 *          number of prime digits is prime.
 * https://oeis.org/A093807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer that is 1) palindromic in base
 *            10, 2) the sum of its decimal digits is a prime, 3) the number
 *            of its prime digits (2,3,5,7) is a prime
 * Range:     u8 n <= 2, u16 n <= 115, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 54 terms through n = 7991 (2026-11-03)
 * Helpers:   digit_frequencies, digit_sum, is_prime_u128, is_prime_u64, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper libraries from the project */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/digit_frequencies.h"
#include "../utils/digit_sum.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/reverse_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Returns the nth term (n ≥ 1) of the sequence */
seq_t a(idx_t n)
{
    /* Count of found terms */
    idx_t count = 0;
    /* Current length of palindromes we are generating */
    idx_t len = 3;               /* Minimum length: 212 */

    /* Loop until we have found the nth term */
    while (1) {
        /* Half length (digits needed to build a palindrome of this length) */
        idx_t half = (len + 1) / 2;

        /* Compute the first prefix: 10^(half-1) */
        idx_t prefix_start = 1;
        for (idx_t i = 1; i < half; ++i) prefix_start *= 10;
        /* Last prefix: 10^half - 1 */
        idx_t prefix_end = prefix_start * 10 - 1;

        /* Precompute powers of 10 needed for constructions */
        seq_t pow10_half   = pow10(half);
        seq_t pow10_halfm1 = (len & 1) ? pow10(half - 1) : 0; /* only for odd lengths */

        /* Iterate over all prefixes of this half length */
        for (idx_t prefix = prefix_start; prefix <= prefix_end; ++prefix) {
            seq_t palindrome;
            if (len & 1)  /* odd length */
            {
                /* Remove the middle digit when mirroring */
                seq_t rev = reverse_digits((seq_t)(prefix / 10));
                palindrome = (seq_t)prefix * pow10_halfm1 + rev;
            } else        /* even length */
            {
                seq_t rev = reverse_digits((seq_t)prefix);
                palindrome = (seq_t)prefix * pow10_half + rev;
            }

            /* ----- check conditions ----- */

            /* Sum of digits must be prime */
            seq_t s = digit_sum(palindrome);
            if (!isPrimeGeneric(s)) continue;

            /* Count of prime digits (2,3,5,7) must be prime */
            unsigned int freq[10] = {0};
            digit_frequencies(palindrome, freq);
            idx_t prime_digit_cnt = freq[2] + freq[3] + freq[5] + freq[7];
            if (!isPrimeGeneric((seq_t)prime_digit_cnt)) continue;

            /* This palindrome meets all criteria */
            ++count;
            if (count == n) return palindrome;
        }

        /* Move to next length */
        ++len;
    }
}
