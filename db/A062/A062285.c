/*
 * A062285  Numbers with the most significant digit even and alternating
 *          parity of digits.
 * https://oeis.org/A062285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 2200, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A062285: numbers with most significant digit even and alternating parity of digits.
   We generate the sequence by brute‑force search, storing produced terms in a static
   cache so subsequent calls only extend the cache.  This approach is O(n) for the
   n̂‑th term and works for all values that fit in seq_t. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;       /* dynamic array holding the sequence          */
static size_t cache_len = 0;      /* number of terms currently stored            */
static size_t cache_cap = 0;      /* current allocation capacity                */
static unsigned long long next_num = 0;   /* next candidate integer to test   */
static bool initialized = false;

/* Return true iff the integer x satisfies the alternating parity rule.
   The function works for numbers that fit in unsigned long long (the
   sequence values are always far below this bound). */
static bool good(unsigned long long x)
{
    if (x == 0)                     /* 0 has msd 0 (even) and no further digits */
        return true;

    /* Find the most significant digit */
    unsigned long long temp = x;
    unsigned long long msd = 0;
    while (temp >= 10) {
        temp /= 10;
    }
    msd = temp;
    if (msd & 1)                     /* msd must be even */
        return false;

    /* Now walk through the digits from msd to lsd checking alternation.
       The loop uses repeated integer division to extract each digit. */
    unsigned long long divisor = 1;
    while (divisor * 10 <= x)
        divisor *= 10;

    unsigned long long last_parity = msd & 1;   /* 0 because msd even */
    bool first = true;
    while (divisor > 0) {
        unsigned long long digit = (x / divisor) % 10;
        if (first) {
            /* The msd has already been checked; just pick up the parity. */
            last_parity = digit & 1;
            first = false;
        } else {
            unsigned long long parity = digit & 1;
            if (parity == last_parity)    /* parity did not alternate */
                return false;
            last_parity = parity;
        }
        divisor /= 10;
    }
    return true;
}

/* Extend the cache until it contains at least n terms. */
static void fill_cache(size_t n)
{
    if (!initialized) {
        /* initialise cache with the first term 0 */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache_len = 1;
        next_num = 0;
        initialized = true;
    }

    while (cache_len < n) {
        next_num++;                     /* advance to next integer to test */
        if (good(next_num)) {
            /* store term */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len] = (seq_t)next_num;
            cache_len++;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* undefined for 0, but return 0 for safety */

    fill_cache((size_t)n);
    return cache[n - 1];
}
