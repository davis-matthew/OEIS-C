/*
 * A044650  Numbers n such that string 2,0 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044650: Numbers n such that the base‑9 representation of n
 * contains the substring 20 and the base‑9 representation of n+1 does not.
 * The function returns the n‑th such integer (offset 1). */

#include <stdbool.h>
#include <stdlib.h>

static bool has20(seq_t x)
{
    seq_t prev = 9;          /* sentinel: not a valid base‑9 digit */
    while (x > 0) {
        seq_t d = x % 9;
        if (prev == 0 && d == 2)   /* 2 (more significant) followed by 0 */
            return true;
        prev = d;
        x /= 9;
    }
    return false;
}

static bool qualifies(idx_t v)
{
    seq_t sv  = (seq_t)v;
    seq_t svp = (seq_t)(v + 1);
    return has20(sv) && !has20(svp);
}

/* Dynamic cache of found terms */
static seq_t *cache     = NULL;   /* array of terms */
static size_t cache_len = 0;      /* number of cached terms */
static size_t cache_cap = 0;      /* allocated capacity */
static idx_t  next_candidate = 1; /* next integer to test */

static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        /* Find next qualifying integer */
        while (!qualifies(next_candidate))
            ++next_candidate;

        /* Grow the cache if needed */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) abort();      /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;   /* start checking from the next integer */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n == 0; return 0 as a fallback */
    ensure_cache(n);
    return cache[n - 1];
}
