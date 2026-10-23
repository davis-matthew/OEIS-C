/*
 * A028981  Primes p such that p+1 is palindromic.
 * https://oeis.org/A028981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 75, u32 n <= 750, u64 n <= 750,
 *            u128 n <= 750, big any n
 * Helpers:   isPrimeGeneric, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028981: primes p such that p+1 is a palindrome (decimal).
   The algorithm generates all decimal palindromic numbers of the form
   p+1, tests p for primality (isPrimeGeneric), and stores each prime
   when it is found.  A cache of found primes allows the function to
   answer any request a(n) in any order of calls. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/isPrimeGeneric.h"
#include "../utils/rev.h"

/* small power of 10 using seq_t arithmetic */
static inline seq_t pow10_seq(unsigned int e)
{
    seq_t r = 1;
    while (e--) r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 */

    /* Cache holding the primes that satisfy the property. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Length of the currently processed palindrome. */
    static idx_t next_len = 1;

    /* Generate primes until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        idx_t L = next_len;                 /* total number of digits in p+1 */
        idx_t half_len = (L + 1) / 2;        /* digits in the first half */

        /* Minimal and maximal left halves (most significant digit non‑zero). */
        idx_t start = 1;
        for (idx_t e = 1; e < half_len; ++e) start *= 10;
        idx_t end = start * 10 - 1;

        /* Multiplier to shift the left half before adding the reversed part. */
        seq_t mul = pow10_seq(half_len - (L % 2));

        for (idx_t i = start; i <= end; ++i)
        {
            /* Build the palindrome as a seq_t. */
            seq_t left = (seq_t)i * mul;

            seq_t rev_part;
            if (L % 2 == 0)
                rev_part = (seq_t)rev(i);
            else
                rev_part = (seq_t)rev(i / 10);

            seq_t pal = left + rev_part;   /* p + 1 */

            if (pal <= 2) continue;        /* p ≤ 1 is not prime */

            seq_t p = pal - 1;
            if (p >= 2 && isPrimeGeneric(p))
            {
                /* Expand cache if necessary. */
                if (cache_len >= cache_cap)
                {
                    cache_cap = cache_cap ? cache_cap * 2 : 4;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = p;
            }
        }
        ++next_len;                         /* move to palindromes of next length */
    }
    return cache[(size_t)(n - 1)];
}
