/*
 * A233878  Number of (n+1)X(3+1) 0..2 arrays with every 2X2 subblock having
 *          the sum of the squares of all six edge and diagonal differences
 *          equal to 11.
 * https://oeis.org/A233878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: 
   a(n) = 56*a(n-2) - 819*a(n-4) + 2791*a(n-6) - 3096*a(n-8) 
          + 961*a(n-10) - 84*a(n-12) 
Initial values: a(1)=240, a(2)=1224, …, a(13)=208076709744 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* memoisation state ----------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size */
static size_t cache_size = 0;  /* largest index that has a non‑zero value */

/* first 13 terms (index 0 unused) --------------------------------------- */
static const seq_t base[14] = {
    0ULL,                 /* unused */
    240ULL,               /* a(1) */
    1224ULL,              /* a(2) */
    6200ULL,              /* a(3) */
    33656ULL,             /* a(4) */
    178704ULL,            /* a(5) */
    1015656ULL,           /* a(6) */
    5566176ULL,           /* a(7) */
    32573400ULL,          /* a(8) */
    181919416ULL,         /* a(9) */
    1082482200ULL,        /* a(10) */
    6108586496ULL,        /* a(11) */
    36673057320ULL,       /* a(12) */
    208076709744ULL       /* a(13) */
};

static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed < 14) needed = 14;      /* keep space for the 13 initial terms */

    if (cache_cap < needed) {
        seq_t *new_cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (cache_cap > 0) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        /* initialise any new entries to zero */
        memset(new_cache + cache_cap, 0, (needed - cache_cap) * sizeof(seq_t));
        cache = new_cache;
        cache_cap = needed;
    }

    /* copy base values into the cache if not already present */
    for (size_t i = 1; i < sizeof(base)/sizeof(base[0]); ++i) {
        if (cache[i] == (seq_t)0)
            cache[i] = base[i];
    }

    if (cache_size < 13)
        cache_size = 13;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                         /* value at index 0 is undefined */
        return (seq_t)0;

    ensure_cache(n);

    /* if already computed, return it */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* compute all unseen terms up to n using the recurrence */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        /* compute positives and negatives separately to avoid
           negative intermediate values when seq_t is unsigned */
        seq_t pos = 56 * cache[i - 2] + 2791 * cache[i - 6] + 961 * cache[i - 10];
        seq_t neg = 819 * cache[i - 4] + 3096 * cache[i - 8] + 84 * cache[i - 12];
        cache[i] = pos - neg;
    }
    if ((size_t)n > cache_size)
        cache_size = (size_t)n;

    return cache[n];
}
