/*
 * A209191  Numbers k such that digital root of k equals smallest prime factor
 *          of k.
 * https://oeis.org/A209191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A209191 : numbers k such that the digital root of k equals its smallest prime factor */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* compute the digital root of a positive integer n
   (works for any non‑negative n fitting in uint64_t) */
static seq_t digital_root(uint64_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t dr = n % 9;
    return dr ? dr : (seq_t)9;
}

/* return the smallest prime factor of n (1 for n==1, otherwise a prime ≤ n) */
static uint64_t spf_uint64(uint64_t n)
{
    if (n < 2)
        return n;            /* 0 or 1 */
    if ((n & 1ULL) == 0)
        return 2U;
    for (uint64_t d = 3; d <= n / d; d += 2)
    {
        if (n % d == 0)
            return d;
    }
    return n;   /* n is prime */
}

/* the sequence function */
seq_t a(idx_t n)
{
    /* offset is 1: a(1) corresponds to the first element stored */
    if (n == 0)
        return (seq_t)0;

    /* static cache of found terms */
    static seq_t *cache     = NULL;   /* array of found terms */
    static size_t cache_size = 0;     /* how many terms are stored */
    static size_t cache_cap  = 0;     /* allocated capacity */
    static uint64_t last_checked = 1; /* last integer examined */

    /* extend the cache until it contains at least n terms */
    while (cache_size < (size_t)n)
    {
        uint64_t k = ++last_checked;           /* next integer to test */

        uint64_t spf_k = spf_uint64(k);
        seq_t    dr    = digital_root(k);

        if ( (uint64_t)spf_k == dr )
        {
            /* ensure enough space in the dynamic array */
            if (cache_cap == 0)
            {
                cache_cap = 16;
                cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            }
            else if (cache_size >= cache_cap)
            {
                cache_cap <<= 1; /* double capacity */
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
