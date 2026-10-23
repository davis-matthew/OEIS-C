/*
 * A158189  Prime numbers p such that the sum of the nonprime digits of p +
 *          the sum of even digits of p is prime.
 * https://oeis.org/A158189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = the n-th prime p such that (sum_{d∈p, d∈{0,1,4,6,8,9}} d)
 *            + (sum_{d∈p, d∈{2,4,6,8}} d) is prime (OEIS A158189)
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime whose sum of non‑prime digits
   plus the sum of even digits is itself prime.  The computation is
   performed by a simple incremental prime search, memoising all
   terms found so far.  */

/* To use the standard prime tester provided by the build system. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* --- helpers ---------------------------------------------------------- */

/* Test if an unsigned 32‑bit number is prime – fast for the small
   sums of digits that we encounter. */
static bool is_small_prime(unsigned int x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (unsigned int i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

/* Return true if the prime satisfies the OEIS condition. */
static bool satisfies_condition(seq_t p)
{
    unsigned int nonprime_sum = 0;
    unsigned int even_sum    = 0;
    uint64_t tmp = (uint64_t)p;          /* work in a wide type */
    while (tmp > 0)
    {
        unsigned int d = (unsigned int)(tmp % 10);
        tmp /= 10;
        if (d == 0 || d == 1 || d == 4 || d == 6 || d == 8 || d == 9)
            nonprime_sum += d;
        if (d == 2 || d == 4 || d == 6 || d == 8)
            even_sum += d;
    }
    unsigned int total = nonprime_sum + even_sum;
    return is_small_prime(total);
}

/* --- state ------------------------------------------------------------- */

static seq_t *cache      = NULL;   /* array of found terms          */
static size_t cache_cap  = 0;      /* allocated size                */
static size_t cache_len  = 0;      /* number of terms stored        */
static idx_t  last_checked = 1;    /* largest integer examined yet */

/* Ensure the cache can hold at least 'needed' entries. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* allocation failure */
    cache      = tmp;
    cache_cap  = new_cap;
}

/* --- entry point ------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined offset 0 */

    size_t requested = (size_t)n;    /* n will be modest for tests */

    /* If we already have the term, return it. */
    if (requested <= cache_len)
        return cache[requested - 1];

    /* Generate terms until we reach the requested index. */
    for (size_t term = cache_len + 1; term <= requested; ++term)
    {
        for (idx_t cand = last_checked + 1; ; ++cand)
        {
            if (is_prime_big((seq_t)cand) && satisfies_condition((seq_t)cand))
            {
                grow_cache(cache_len + 1);
                cache[cache_len++] = (seq_t)cand;
                last_checked = cand;
                break;
            }
        }
    }

    return cache[requested - 1];
}
