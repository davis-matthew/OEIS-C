/*
 * A169754  Numbers of the form k^k which contain k as a substring in base 10.
 * https://oeis.org/A169754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A169754:
   a(n) = the nth integer of the form k^k that contains k as a decimal
   substring.  The first term (n = 1) is 1.
   The implementation searches k = 1,2,3,… in order, keeps a cache of
   the discovered terms, and returns the requested one.  The cache is
   maintained across calls so that non‑monotonic request patterns are
   handled efficiently.  Only standard C11 and C++17 facilities are used
   and the code respects the restrictions on seq_t and idx_t. */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uintmax_t */
#include <stdio.h>    /* snprintf */
#include <string.h>   /* strstr */
#include <stdlib.h>   /* malloc, realloc */
#include <math.h>     /* log10 */

#include "../utils/big_pow.h"      /* seq_t big_pow(seq_t, seq_t); */
#include "../utils/seq_to_str.h"   /* void seq_to_str(seq_t, char*, size_t*); */

static seq_t *cache = NULL;       /* cache of discovered terms      */
static size_t cache_len  = 0;      /* number of cached terms         */
static size_t cache_cap  = 0;      /* allocated cache capacity       */

static idx_t next_k = 1;           /* next value of k to test        */

static char *powbuf = NULL;        /* buffer for decimal string of k^k */
static size_t powbuf_cap = 0;      /* allocated size of powbuf        */

/* Grow the cache if additional 'need' terms are required. */
static void ensure_cache(size_t need)
{
    if (cache_len + need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < cache_len + need)
        new_cap = cache_len + need;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp)
        abort();
    cache = newp;
    cache_cap = new_cap;
}

/* Grow the power buffer if additional 'need' bytes are required. */
static void ensure_powbuf(size_t need)
{
    if (powbuf_cap >= need)
        return;
    size_t new_cap = powbuf_cap ? powbuf_cap * 2 : 1;
    while (new_cap < need)
        new_cap *= 2;
    char *newp = (char *)realloc(powbuf, new_cap);
    if (!newp)
        abort();
    powbuf = newp;
    powbuf_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* safety for out‑of‑range requests */
        return (seq_t)0;

    if ((size_t)n <= cache_len) /* already computed */
        return cache[n - 1];

    /* Temporary buffer for the decimal representation of k. */
    char kbuf[32];

    /* Generate terms until we have the requested one. */
    for ( ; cache_len < (size_t)n; ++next_k )
    {
        /* Decimal string of k for substring test. */
        snprintf(kbuf, sizeof(kbuf), "%ju", (uintmax_t)next_k);

        /* Compute k^k. */
        seq_t power = big_pow((seq_t)next_k, (seq_t)next_k);

        /* Rough estimate of the number of decimal digits of k^k. */
        size_t est_digits = (size_t)(next_k * log10((double)next_k) + 15);
        ensure_powbuf(est_digits + 2);   /* +2 for safety and NUL */

        /* Convert the number to a string. */
        size_t out_len = powbuf_cap;
        seq_to_str(power, powbuf, &out_len);
        powbuf[out_len] = '\0';

        /* Test for the presence of k as a substring. */
        if (strstr(powbuf, kbuf))
        {
            ensure_cache(1);
            cache[cache_len++] = power;
        }
        /* If not selected, the value is discarded automatically. */
    }

    return cache[n - 1];
}
