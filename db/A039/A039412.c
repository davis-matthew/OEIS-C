/*
 * A039412  Numbers whose base-10 representation has the same nonzero number
 *          of 2's and 5's.
 * https://oeis.org/A039412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th natural number whose decimal representation has
 *            the same nonzero number of 2's and 5's
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function must be valid in both C11 and C++17.
   seq_t and idx_t are provided by the build. */

#include <stdlib.h>
#include <string.h>

/* Count all decimal digits of x into cnt[0..9].  The count may
   exceed 255, but for the purposes of this sequence only the values
   for digits 2 and 5 are used, so 8 bits per digit are sufficient. */
static void digit_counts(seq_t x, unsigned char cnt[10])
{
    for (int i = 0; i < 10; ++i)
        cnt[i] = 0;
    do
    {
        /* x % 10 and x / 10 are defined for all supported seq_t types */
        unsigned d = (unsigned)(x % 10);
        cnt[d] += 1;
        x /= 10;
    } while (x != 0);
}

/* Cached results allow independent, non‑monotonic calls. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* The main sequence generator. */
seq_t a(idx_t n)
{
    /* b(1) = 25, so the function is defined only for n ≥ 1 */
    if (n < 1)
        return 0;

    /* Initialise the cache on first call.  A small initial capacity
       reduces the number of reallocations for early indices. */
    if (!cache)
    {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n elements. */
    while (cache_len < (size_t)n)
    {
        seq_t cand = (cache_len == 0) ? 25 : cache[cache_len - 1] + 1;

        /* Search for the next valid candidate. */
        while (1)
        {
            unsigned char cnt[10];
            digit_counts(cand, cnt);
            if (cnt[2] && cnt[5] && cnt[2] == cnt[5])
                break;
            cand += 1;
        }

        /* Ensure sufficient capacity.  Use exponential growth. */
        if (cache_len >= cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_buf)   /* allocation failure: abort */
                abort();
            cache = new_buf;
            cache_cap = new_cap;
        }

        cache[cache_len++] = cand;
    }

    return cache[n - 1];
}
