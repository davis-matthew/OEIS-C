/*
 * A202118  Number of -2..2 arrays of n elements with first, second and third
 *          differences also in -2..2.
 * https://oeis.org/A202118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-3) + 3*a(n-4) - 2*a(n-5) - 3*a(n-6) -
 *            4*a(n-7) - 3*a(n-8) - 2*a(n-9) + 3*a(n-10) + 4*a(n-11) +
 *            2*a(n-12) (for n > 17) Initial values (n = 1 … 17): 5, 19, 57,
 *            127, 293, 663, 1517, 3459, 7905, 18051, 41239, 94203, 215217,
 *            491661, 1123219, 2566023, 5862189
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 24, u64 n <= 51, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* malloc, realloc, abort */
#include <stddef.h>    /* size_t */

static const seq_t init_vals[17] = {
    (seq_t)5,      (seq_t)19,      (seq_t)57,      (seq_t)127,
    (seq_t)293,    (seq_t)663,      (seq_t)1517,    (seq_t)3459,
    (seq_t)7905,   (seq_t)18051,    (seq_t)41239,   (seq_t)94203,
    (seq_t)215217, (seq_t)491661,   (seq_t)1123219, (seq_t)2566023,
    (seq_t)5862189
};

static seq_t *cache      = NULL;   /* cache[i] holds a(i+1)          */
static size_t cache_cap  = 0;      /* allocated capacity (number of seq_t)   */
static size_t cache_len  = 0;      /* number of terms currently computed   */

/* Grow the cache so that it can hold at least `need` elements.  */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = (cache_cap == 0) ? 17 : cache_cap * 2;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort(); /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the n-th term of A202118 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)       /* undefined by OEIS; return 0 as safe sentinel */
        return (seq_t)0;

    /* Convert idx_t to size_t safely; n >= 1 so it fits. */
    size_t m = (size_t)n;

    if (m <= cache_len)
        return cache[m - 1];

    /* Ensure we have space for the new terms. */
    ensure_cache(m);

    /* Compute missing terms up to n.  We fill entries cache_len … m-1. */
    for (size_t i = cache_len; i < m; ++i) {
        size_t idx = i + 1;      /* 1‑based index of the term being computed */

        if (idx <= 17) {
            cache[i] = init_vals[idx - 1];
            continue;
        }

        /* Positive part of the recurrence. */
        seq_t pos = 0;
        pos += (seq_t)2   * cache[idx - 2]; /* 2*a(n-1)          */
        pos +=              cache[idx - 4]; /* a(n-3)            */
        pos += (seq_t)3   * cache[idx - 5]; /* 3*a(n-4)          */
        pos += (seq_t)3   * cache[idx - 11];/* 3*a(n-10)         */
        pos += (seq_t)4   * cache[idx - 12];/* 4*a(n-11)         */
        pos += (seq_t)2   * cache[idx - 13];/* 2*a(n-12)         */

        /* Negative part of the recurrence. */
        seq_t neg = 0;
        neg += (seq_t)2   * cache[idx - 6]; /* 2*a(n-5)          */
        neg += (seq_t)3   * cache[idx - 7]; /* 3*a(n-6)          */
        neg += (seq_t)4   * cache[idx - 8]; /* 4*a(n-7)          */
        neg += (seq_t)3   * cache[idx - 9]; /* 3*a(n-8)          */
        neg += (seq_t)2   * cache[idx - 10];/* 2*a(n-9)          */

        cache[i] = pos - neg;  /* The recurrence guarantees this is non‑negative */
    }

    cache_len = m;  /* we have computed up to term n */
    return cache[m - 1];
}
