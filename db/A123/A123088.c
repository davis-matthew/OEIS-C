/*
 * A123088  a(1)=1. For n>=2, a(n) = n + (largest integer which is <= n and is
 *          missing from the earlier terms of the sequence).
 * https://oeis.org/A123088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A123088.
   a(1) = 1.
   For n>=2, a(n) = n + L, where L is the largest integer <= n
   that does not appear among a(1),...,a(n-1).  The function
   memoizes computed terms so that calls in any order are fast. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------------ */
/*  Cache for computed terms                                             */
/* ------------------------------------------------------------------ */
#ifdef __cplusplus
static std::vector<seq_t> cache;
static idx_t max_n_computed = 0;
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_n_computed = 0;
#endif

/* Ensure that 'cache' can hold at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
#ifdef __cplusplus
    if (need <= cache.size()) return;
    cache.reserve(need);
    cache.resize(need);
#else
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
#endif
}

/* ------------------------------------------------------------------ */
/*  Bitmap of values seen among earlier terms of the sequence          */
/* ------------------------------------------------------------------ */
static bool *seen = NULL;
static size_t seen_cap = 0;

/* Ensure that 'seen' can index up to 'need' (1‑based).  Elements 0
   are unused. */
static void ensure_seen_cap(size_t need)
{
    if (need <= seen_cap) return;
    size_t old = seen_cap;
    size_t new_cap = seen_cap ? seen_cap << 1 : 4;
    while (new_cap < need) new_cap <<= 1;
    bool *tmp = (bool*)realloc(seen, new_cap * sizeof(bool));
    if (!tmp) abort();
    /* Zero the newly allocated part. */
    memset(tmp + old, 0, (new_cap - old) * sizeof(bool));
    seen = tmp;
    seen_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for 0, safety */

    /* Compute all terms up to n if necessary. */
    for (idx_t i = max_n_computed + 1; i <= n; ++i)
    {
        seq_t val;

        if (i == 1)
        {
            val = (seq_t)1;
        }
        else
        {
            /* Prepare bitmap up to i. */
            ensure_seen_cap((size_t)i + 1);

            /* Find largest missing integer <= i. */
            size_t missing = 0;
            for (size_t cand = (size_t)i; cand > 0; --cand)
            {
                if (!seen[cand])
                {
                    missing = cand;
                    break;
                }
            }

            val = (seq_t)i + (seq_t)missing;
        }

        /* Store the term. */
        ensure_cache_cap((size_t)i);
#ifdef __cplusplus
        cache[i - 1] = val;
#else
        cache[i - 1] = val;
#endif

        /* Mark this value in the bitmap, if it fits into size_t. */
        if (val <= (seq_t)SIZE_MAX)
        {
            size_t v = (size_t)val;
            if (v > 0)           /* ignore zero */
            {
                ensure_seen_cap(v + 1);
                seen[v] = true;
            }
        }

        max_n_computed = i;
    }

    return cache[n - 1];
}
