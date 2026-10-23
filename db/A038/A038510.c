/*
 * A038510  Composite numbers with smallest prime factor >= 7.
 * https://oeis.org/A038510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A038510: Composite numbers with smallest prime factor ≥ 7.
   The function builds a list of such numbers and returns the nth term.
   It uses a simple sieve by trial division, generating numbers in order.
*/

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* memoized terms */
static size_t      cache_size   = 0;  /* number of terms stored */
static size_t      cache_cap    = 0;  /* capacity of cache array */
static seq_t       next_candidate = (seq_t)49;   /* next number to test */

static void ensure_capacity(size_t needed)
{
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < needed) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Compute the nth term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety, though not used */

    /* Generate terms until we have at least n of them. */
    while (cache_size < n)
    {
        seq_t cand = next_candidate;

        /* Skip numbers divisible by 2, 3 or 5. */
        while (cand % (seq_t)2 == (seq_t)0
            || cand % (seq_t)3 == (seq_t)0
            || cand % (seq_t)5 == (seq_t)0)
        {
            cand += (seq_t)1;
        }

        /* Test whether cand is composite and has no factor 2, 3, or 5. */
        bool composite = false;
        if (cand >= (seq_t)49)   /* 7*7 is the smallest such composite */
        {
            for (seq_t p = (seq_t)7; p * p <= cand; p += (seq_t)2)
            {
                if (cand % p == (seq_t)0)
                {
                    composite = true;
                    break;
                }
            }
        }

        if (composite)
        {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = cand;
        }

        /* Prepare the next candidate for the next iteration. */
        next_candidate = cand + (seq_t)1;
    }

    return cache[n - 1];
}
