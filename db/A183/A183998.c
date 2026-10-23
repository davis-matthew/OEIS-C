/*
 * A183998  Number of (n+1) X 5 0..2 arrays with all 2 X 2 subblock sums the
 *          same.
 * https://oeis.org/A183998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A183998: Number of (n+1)×5 arrays with entries 0..2
   such that all 2×2 subblock sums are equal.
   Recurrence:
     a(n) = 6a(n‑1) – 60a(n‑3) + 85a(n‑4) + 150a(n‑5) – 360a(n‑6)
            + 396a(n‑8) – 216a(n‑9)       (n ≥ 10)
   Initial values for 1 ≤ n ≤ 9:
     a(1) = 855,  a(2) = 1233,  a(3) = 1887,  a(4) = 3309,
     a(5) = 6111,  a(6) = 12525, a(7) = 26655, a(8) = 61125,
     a(9) = 144495 */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache      = NULL;   /* cache[0] unused */
static size_t cache_cap  = 0;       /* number of allocated slots */
static size_t cache_len  = 0;       /* highest index stored (0 if none) */

/* Ensure the cache has space for indices 0 .. need-1. */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t newcap = cache_cap ? cache_cap << 1 : 16;
    while (newcap < need)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache       = tmp;
    cache_cap   = newcap;
}

/* Initialise cache with the first nine terms. */
static void init_cache(void)
{
    static const seq_t init_vals[10] = {
        (seq_t)0,        /* cache[0] unused */
        (seq_t)855,      /* a(1) */
        (seq_t)1233,     /* a(2) */
        (seq_t)1887,     /* a(3) */
        (seq_t)3309,     /* a(4) */
        (seq_t)6111,     /* a(5) */
        (seq_t)12525,    /* a(6) */
        (seq_t)26655,    /* a(7) */
        (seq_t)61125,    /* a(8) */
        (seq_t)144495    /* a(9) */
    };
    ensure_cache(10);             /* indices 0..9 */
    for (idx_t i = 1; i <= 9; ++i)
        cache[i] = init_vals[i];
    cache_len = 9;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n==0 */

    if (!cache)
        init_cache();

    if ((size_t)n <= cache_len)
        return cache[n];          /* already computed */

    ensure_cache((size_t)n + 1);   /* need indices 0..n */

    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        seq_t val = 0;
        /* recurrence applies only for i ≥ 10; for 10..n all indices are valid */
        /*   a(n) = 6a(n‑1) – 60a(n‑3) + 85a(n‑4) + 150a(n‑5) – 360a(n‑6)
                 + 396a(n‑8) – 216a(n‑9)                           */
        val += (seq_t)6   * cache[i - 1];
        val -= (seq_t)60  * cache[i - 3];
        val += (seq_t)85  * cache[i - 4];
        val += (seq_t)150 * cache[i - 5];
        val -= (seq_t)360 * cache[i - 6];
        val += (seq_t)396 * cache[i - 8];
        val -= (seq_t)216 * cache[i - 9];
        cache[i] = val;
    }
    cache_len = (size_t)n;
    return cache[(size_t)n];
}
