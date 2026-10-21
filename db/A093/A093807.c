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
 * Helpers:   digit_frequencies, digit_sum, isPrimeGeneric, pow10, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper libraries from the project */
#include "../utils/pow10.h"         /* seq_t pow10(idx_t e) */
#include "../utils/rev_decimal.h"   /* seq_t rev_decimal(seq_t v) */
#include "../utils/digit_sum.h"     /* seq_t digit_sum(seq_t v) */
#include "../utils/digit_frequencies.h" /* void digit_frequencies(seq_t x, unsigned int freq[10]) */
#include "../utils/isPrimeGeneric.h" /* bool isPrimeGeneric(seq_t n) */

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
                seq_t rev = rev_decimal((seq_t)(prefix / 10));
                palindrome = (seq_t)prefix * pow10_halfm1 + rev;
            } else        /* even length */
            {
                seq_t rev = rev_decimal((seq_t)prefix);
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
