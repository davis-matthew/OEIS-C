/*
 * A097703  Numbers j such that m = 216*j + 108 satisfies sigma(m) !=
 *          2*usigma(m).
 * https://oeis.org/A097703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer j ≥ 1 for which (2*j+1) is divisible by
 *            3 or (2*j+1) is not square‑free. This is equivalent to the
 *            definition given for OEIS A097703
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static cache for memoisation */
static seq_t *cache      = NULL;   /* cached values of a(n) */
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t  last_checked_j = 0;  /* last j examined for the cache */

/* Grow the cache array to hold at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);            /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return true iff n is square‑free.  n is guaranteed to fit in 64 bits
   for all practical n required by the sequence (n ≈ 2 * 10⁴). */
static bool is_squarefree(uint64_t n)
{
    if (n <= 1) return true;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            n /= p;
            if (n % p == 0) return false;   /* repeated prime factor */
        }
    }
    return true;
}

/* The sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety guard; OEIS offset is 1 */

    /* Ensure the cache holds at least the first n terms. */
    while (cache_len < (size_t)n)
    {
        /* Consider the next candidate j. */
        ++last_checked_j;
        uint64_t val = 2ull * last_checked_j + 1;   /* 2*j+1 */

        /* Condition: (2*j+1) divisible by 3 OR not square‑free. */
        bool cond = (val % 3 == 0) || !is_squarefree(val);

        if (cond)
        {
            ensure_cache(cache_len + 1);
            cache[cache_len] = (seq_t)last_checked_j;
            ++cache_len;
        }
    }

    /* Offset of a(1) is 1, so cache[0] holds a(1). */
    return cache[n - 1];
}
