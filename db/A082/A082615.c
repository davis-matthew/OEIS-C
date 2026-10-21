/*
 * A082615  Palindromes such that the GCD of any pair of successive terms is a
 *          palindromic prime.
 * https://oeis.org/A082615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 333, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_palindrome, is_prime_big, seq_from_str, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A082615 – Palindromes such that the GCD of any pair of successive terms
    is a palindromic prime.
    The function computes a(n) for any n ≥ 1, using memoisation and
    generating the next required palindrome efficiently.  */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Utility functions supplied by the build system */
#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"
#include "../utils/is_palindrome.h"
#include "../utils/seq_to_str.h"
#include "../utils/seq_from_str.h"

/* ------------------------------------------------------------------ */
/*  Cache of already computed terms                                    */
/* ------------------------------------------------------------------ */
static seq_t *cache      = NULL;
static idx_t cache_len   = 0;   /* number of entries in the cache   */
static size_t cache_cap  = 0;   /* allocated capacity of the cache  */

/* Ensure the cache holds at least 'cap' elements.  */
static void
ensure_cache_cap (size_t cap)
{
    if (cap <= cache_cap)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
    if (new_cap < cap)
        new_cap = cap;
    seq_t *newp = (seq_t *)realloc (cache, new_cap * sizeof (seq_t));
    if (!newp) {
        if (cache)
            free (cache);
        abort ();
    }
    cache = newp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Generate the smallest palindrome strictly greater than n.         */
/* ------------------------------------------------------------------ */
static seq_t
next_palindrome (seq_t n)
{
    /* Start searching at n+1.  */
    seq_t candidate = n + (seq_t)1;
    for (;;) {
        /* Convert the candidate to a decimal string.  */
        char buf[256];
        size_t len;
        seq_to_str (candidate, buf, &len);
        if (len + 1 >= sizeof (buf)) /* safety guard – should not happen */
            return candidate;       /* fallback – unlikely for the problem size */

        buf[len] = '\0';

        /* Build the mirrored version.  */
        char pal[256];
        memcpy (pal, buf, len + 1);               /* copy including NUL */
        for (size_t i = 0; i < len; ++i)
            pal[len - 1 - i] = pal[i];

        /* If this palindrome is > the original number, we are done.  */
        if (memcmp (pal, buf, len) > 0) {
            return seq_from_str (pal);
        }

        /* Otherwise we need to increment the middle part and mirror again.  */
        int carry = 1;
        int pos = (int)len / 2 - (int)(len % 2 == 0);
        for (int i = pos; i >= 0 && carry; --i) {
            int d = (pal[i] - '0') + carry;
            carry = d / 10;
            pal[i] = '0' + (d % 10);
        }

        if (carry) {
            /* Handle overflow (e.g. 999 → 1001).  */
            char big[256];
            big[0] = '1';
            for (int i = 1; i < (int)len; ++i)
                big[i] = '0';
            big[len] = '1';
            big[len + 1] = '\0';
            return seq_from_str (big);
        } else {
            /* Mirror again after the increment.  */
            for (int i = 0; i < (int)len; ++i)
                pal[len - 1 - i] = pal[i];
            pal[len] = '\0';
            return seq_from_str (pal);
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Main function implementing the sequence.                          */
/* ------------------------------------------------------------------ */
seq_t a (idx_t n)
{
    if (n == 0)            /* undefined for n < 1, harmless as per spec */
        return (seq_t)0;

    /* Grow the cache until it contains term n.  */
    if (cache_len < n) {
        ensure_cache_cap (n);

        /* Initialise the first term if we are starting from scratch. */
        if (cache_len == 0) {
            cache[0] = (seq_t)2;
            cache_len = 1;
        }

        /* Compute remaining terms up to n.  */
        for (idx_t i = cache_len; i < n; ++i) {
            seq_t prev = cache[i - 1];

            /* Start with the next palindrome after prev. */
            seq_t cand = next_palindrome (prev);

            /* Search until we find a palindrome whose GCD with prev
               is a palindromic prime.  */
            while (true) {
                seq_t g = gcd (prev, cand);
                if (g > (seq_t)1 && is_prime_big (g) && is_palindrome (g, (idx_t)10))
                    break;
                cand = next_palindrome (cand);
            }

            cache[i] = cand;
            cache_len = i + 1;
        }
    }

    return cache[n - 1];
}
