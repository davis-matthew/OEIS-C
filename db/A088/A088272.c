/*
 * A088272  Palindromic primes that yield a prime when sandwiched between two
 *          9's.
 * https://oeis.org/A088272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth palindromic prime p such that the decimal number
 *            formed by sandwiching p between two 9's (i.e. 9p9) is also
 *            prime. The function searches integers in ascending order, tests
 *            for palindrome, primality of p, and primality of 9p9
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   append_candidate, digit_len, is_palindrome, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_palindrome.h"
#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* The next integer to test */
    static idx_t next_candidate = 2;

    /* If the cache already covers the requested index, return it */
    if (cache_len >= n)
        return cache[n - 1];

    /* Generate terms until we have at least n */
    for (idx_t x = next_candidate; ; ++x)
    {
        seq_t p = (seq_t)x;

        /* Skip non‑palindromes quickly */
        if (!is_palindrome(p, 10))
            continue;

        /* Must be a prime itself */
        if (!is_prime_big(p))
            continue;

        /* Build 9p9 */
        int d = digit_len(p);            /* number of decimal digits of p */
        seq_t t = pow10((idx_t)(d + 1));  /* 10^(d+1) */
        seq_t sandwich = ((seq_t)9) * t + p * ((seq_t)10) + ((seq_t)9);

        /* 9p9 must also be prime */
        if (!is_prime_big(sandwich))
            continue;

        /* Store the qualifying prime */
        append_candidate(&cache, &cache_len, &cache_cap, p);

        /* If we reached the desired term, stop */
        if (cache_len >= n) {
            next_candidate = x + 1;
            break;
        }
    }
    return cache[n - 1];
}
