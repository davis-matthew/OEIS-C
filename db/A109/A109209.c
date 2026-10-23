/*
 * A109209  Palindromic primes pp such that digit sum of pp as substring
 *          appears more than once in pp.
 * https://oeis.org/A109209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   has_two_nonoverlap_sum, is_prime, make_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A109209: palindromic primes for which the decimal digit sum
   appears at least twice as a *non‑overlapping* substring inside the
   palindrome.  The function generates palindromes in increasing numeric
   order by length and prefix, tests the digit‑sum substring condition
   (with non‑overlap) and primality, counting until the nth valid
   element is reached. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/has_two_nonoverlap_sum.h"
#include "../utils/make_palindrome.h"

static const uint64_t pow10_ull[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL,
    100000ULL, 1000000ULL, 10000000ULL,
    100000000ULL, 1000000000ULL, 10000000000ULL,
    100000000000ULL, 1000000000000ULL, 10000000000000ULL,
    100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL, 1000000000000000000ULL, 10000000000000000000ULL
};

/* External linkage required by the specification */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n=0, but safe return */
        return (seq_t)0;

    idx_t found = 0;
    seq_t result = (seq_t)0;

    for (int len = 1; ; ++len) {          /* increasing length */
        int half = (len + 1) / 2;          /* digits in prefix */
        uint64_t start = pow10_ull[half - 1];
        uint64_t end   = pow10_ull[half] - 1;
        for (uint64_t pref = start; pref <= end; ++pref) {
            uint64_t pal = make_palindrome(pref, len);
            if (!has_two_nonoverlap_sum(pal))
                continue;
            if (!is_prime(pal))
                continue;
            ++found;
            if (found == n) {
                return (seq_t)pal;
            }
        }
    }
}
