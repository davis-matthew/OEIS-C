/*
 * A083840  Palindromic primes which are a member of a twin prime pair.
 * https://oeis.org/A083840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth palindromic prime that is part of a twin‑prime
 *            pair. The code generates all palindromic numbers in increasing
 *            order, tests primality (is_prime_big) and the twin condition,
 *            and returns the nth matching value
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 23, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   is_prime_big, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper headers */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/reverse_digits.h"

/* 10^exp where exp is a nonnegative idx_t.
   Exponent is small (≤ 20 for the sequence up to 10‑000 terms). */
static idx_t pow10(idx_t exp)
{
    idx_t r = 1;
    for (idx_t i = 0; i < exp; ++i) r *= 10;
    return r;
}

/* Return the nth element of A083840. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for <1, harmless */

    idx_t found = 0;        /* how many values found so far */

    /* Enumerate palindromes by increasing length. */
    for (idx_t length = 1; /* infinite */ ; ++length)
    {
        /* Handle length 1 separately (single‑digit palindromes). */
        if (length == 1)
        {
            /* left part has 1 digit: 1 … 9 */
            for (idx_t left = 1; left <= 9; ++left)
            {
                seq_t value = (seq_t)left;   /* single digit */
                if (!is_prime_big(value)) continue;

                seq_t minus2 = value - (seq_t)2;
                seq_t plus2  = value + (seq_t)2;

                if (!is_prime_big(minus2) && !is_prime_big(plus2))
                    continue;

                ++found;
                if (found == n) return value;
            }
            continue;
        }

        /* For length ≥ 2 */
        if (length & 1)   /* odd length: 2k+1 */
        {
            idx_t k = (length - 1) / 2;        /* number of digits on the left half minus 1 */
            idx_t start = pow10(k);            /* smallest left part with k+1 digits */
            idx_t end   = pow10(k + 1) - 1;    /* largest left part with k+1 digits */
            seq_t pow10k = (seq_t)pow10(k);

            for (idx_t left = start; left <= end; ++left)
            {
                seq_t leftNum = (seq_t)left;
                seq_t rev = reverse_digits(leftNum / (seq_t)10);  /* omit middle digit */
                seq_t value = leftNum * pow10k + rev;

                if (!is_prime_big(value)) continue;

                seq_t minus2 = value - (seq_t)2;
                seq_t plus2  = value + (seq_t)2;

                if (!is_prime_big(minus2) && !is_prime_big(plus2))
                    continue;

                ++found;
                if (found == n) return value;
            }
        }
        else            /* even length: 2k */
        {
            idx_t k = length / 2;                /* number of digits on each half */
            idx_t start = pow10(k - 1);           /* smallest left part with k digits */
            idx_t end   = pow10(k) - 1;           /* largest left part with k digits */
            seq_t pow10k = (seq_t)pow10(k);

            for (idx_t left = start; left <= end; ++left)
            {
                seq_t leftNum = (seq_t)left;
                seq_t rev = reverse_digits(leftNum);
                seq_t value = leftNum * pow10k + rev;

                if (!is_prime_big(value)) continue;

                seq_t minus2 = value - (seq_t)2;
                seq_t plus2  = value + (seq_t)2;

                if (!is_prime_big(minus2) && !is_prime_big(plus2))
                    continue;

                ++found;
                if (found == n) return value;
            }
        }
    }
}
