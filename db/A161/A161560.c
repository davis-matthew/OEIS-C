/*
 * A161560  a(n) = floor(A000069(n)/2-1/2).
 * https://oeis.org/A161560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000069(n) - n (equivalently floor(A000069(n)/2-1/2))
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper which returns 1 if a value has an odd number of 1‐bits (odious) */
static inline int is_odious(seq_t x)
{
    int pc = 0;
    while (x)
    {
        pc ^= (int)(x & 1);
        x >>= 1;
    }
    return pc;
}

/* Cache for previously computed values of the sequence. */
static seq_t *cache = NULL;
static idx_t  cache_cap  = 0;   /* total allocated slots (1‑based indices) */
static idx_t  cached_n   = 0;   /* largest n that has already been computed */
static seq_t  next_candidate = 1;   /* candidate integer to test for odiousness */

/* Make sure the cache holds at least 'needed' entries. */
static void ensure_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);                 /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* External function implementing A161560.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)         /* undefined for n < 1 – return 0 by convention */
        return 0;

    /* Compute any missing values up to n. */
    if (n > cached_n)
    {
        ensure_cache(n);
        while (cached_n < n)
        {
            /* Find the next odious number. */
            while (!is_odious(next_candidate))
                ++next_candidate;

            /* Store a(cached_n+1) = odious - (cached_n+1). */
            cache[cached_n] =
                next_candidate - (seq_t)(cached_n + 1);

            ++next_candidate;     /* next integer to examine */
            ++cached_n;
        }
    }
    return cache[n - 1];
}
