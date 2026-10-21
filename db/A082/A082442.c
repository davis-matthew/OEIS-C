/*
 * A082442  Palindromic primes with middle digit 6.
 * https://oeis.org/A082442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 2, u64 n <= 400, u128 n <= 400,
 *            big any n
 * Helpers:   is_prime_big, pow10, rev_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) is the n‑th palindromic prime whose length is odd
   and whose middle digit is 6.  The function generates palindromes
   incrementally, tests for primality using is_prime_big, and returns
   the requested term.  The algorithm runs in O(m) time where m is the
   number of candidates examined. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/rev_digits_uint64.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;           /* number of valid primes found */

    /* len_left = number of digits in the left half of the odd‑length
        palindrome (excluding the middle digit 6). */
    for (idx_t len_left = 1;; ++len_left) {
        /* smallest and largest values for the left half; no leading zeros */
        idx_t start = (len_left == 1) ? 1 : (idx_t)pow10(len_left - 1);
        idx_t end   = (idx_t)pow10(len_left) - 1;

        /* 10^len_left  and  10^(len_left+1) as seq_t for assembling
           the full palindrome.  Reuse these values for every left half
           of the current length. */
        seq_t pow_len_left      = pow10(len_left);      /* 10^len_left      */
        seq_t pow_len_left_plus = pow10(len_left + 1);  /* 10^(len_left+1) */

        for (idx_t L = start; L <= end; ++L) {
            /* reverse the left half to form the right half */
            seq_t rev = rev_digits_uint64(L);

            /* build the full palindrome:
                 [left] 6 [reverse(left)]  */
            seq_t val = (seq_t)L * pow_len_left_plus
                       + (seq_t)6 * pow_len_left
                       + rev;

            if (is_prime_big(val)) {
                ++count;
                if (count == n) {
                    return val;
                }
            }
        }
    }
}
