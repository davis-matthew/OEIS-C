/*
 * A214586  Numbers k such that gcd(k!!+1,k-1) = 1.
 * https://oeis.org/A214586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence defined by: a(n) = the n-th even integer k such that gcd(k!! + 1, k-1) = 1 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;
static size_t cap = 0;
static size_t filled = 0;

/* Ensure the cache can hold at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cap) return;
    size_t newcap = cap ? cap << 1 : 64;
    while (newcap < need) newcap <<= 1;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cap = newcap;
}

/* Test whether a given even k satisfies gcd(k!! + 1, k-1) = 1. */
static bool good_k(seq_t k)
{
    /* m = k-1  (always odd for even k) */
    seq_t m = k - (seq_t)1;
    seq_t rem = (seq_t)1;
    for (seq_t e = (seq_t)2; e <= k; e = e + (seq_t)2)
    {
        rem = (rem * e) % m;
    }
    seq_t r = (rem + (seq_t)1) % m;          /* (k!! + 1) mod (k-1)   */
    return gcd(m, r) == (seq_t)1;
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;      /* undefined but harmless */

    if (filled >= (size_t)n) return cache[n - 1];

    /* start searching from the integer after the last stored k */
    seq_t current = (filled > (size_t)0) ? (cache[filled - 1] + (seq_t)1) : (seq_t)2;

    while (filled < (size_t)n)
    {
        if ((current & (seq_t)1) == (seq_t)0)   /* only even k need be tested */
        {
            if (good_k(current))
            {
                grow_cache(filled + 1);
                cache[filled++] = current;
            }
        }
        current = current + (seq_t)1;
    }

    return cache[n - 1];
}
