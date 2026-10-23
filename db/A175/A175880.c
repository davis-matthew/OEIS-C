/*
 * A175880  a(1)=1, a(2)=2. If n >= 3: if n/2 is in the sequence, a(n)=0,
 *          otherwise a(n)=n.
 * https://oeis.org/A175880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175880
   a(1) = 1, a(2) = 2.
   For n >= 3:
     if n is odd                     -> a(n) = n
     if n is even and a(n/2) != 0    -> a(n) = 0
     otherwise (n/2 not in the sequence) -> a(n) = n
   This implementation lazily computes terms and memoises them in a
   static array so that repeated calls are fast. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* holds a(1)...a(cache_len) */
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* enlarge the cache to accommodate at least n elements */
static void ensure_cache_cap(idx_t n)
{
    if (cache_cap < (size_t)n)
    {
        size_t new_cap = (n > 4) ? n * 2 : 4;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* compute all terms up to n (inclusive) */
static void compute_to(idx_t n)
{
    ensure_cache_cap(n);
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        seq_t val;
        if (i == 1)
            val = (seq_t)1;
        else if (i == 2)
            val = (seq_t)2;
        else if ((i & 1) == 1)                     /* odd i */
            val = (seq_t)i;
        else
        {
            idx_t half = i / 2;
            seq_t half_val = cache[half - 1];
            if (half_val != (seq_t)0)
                val = (seq_t)0;
            else
                val = (seq_t)i;
        }
        cache[i - 1] = val;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* off‑by‑one safety */
    if (cache_len < n) compute_to(n);
    return cache[n - 1];
}
