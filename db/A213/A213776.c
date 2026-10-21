/*
 * A213776  Antidiagonal sums of the convolution array A213774.
 * https://oeis.org/A213776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5) for n >=
 *            6. Initial values: a(1)=1, a(2)=8, a(3)=30, a(4)=81, a(5)=184
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* memoisation array, 0‑based: a(1) at cache[0] */
static size_t  cap    = 0;       /* current allocated capacity */
static idx_t   last   = 0;       /* maximum n that has been computed */

/* Ensure the cache can hold at least n elements (n >= 1). */
static void grow_cache(idx_t n)
{
    if (cap >= (size_t)n)
        return;
    size_t new_cap = cap ? cap : 16;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();               /* allocation failure */
    if (cache) {
        memcpy(new_cache, cache, cap * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cap   = new_cap;
}

/* Initialise cache with the first five terms. */
static void init_cache(void)
{
    grow_cache(16);           /* allow some spare space */
    cache[0] = (seq_t)1;      /* a(1) */
    cache[1] = (seq_t)8;      /* a(2) */
    cache[2] = (seq_t)30;     /* a(3) */
    cache[3] = (seq_t)81;     /* a(4) */
    cache[4] = (seq_t)184;    /* a(5) */
    last = 5;                 /* highest computed index */
}

seq_t a(idx_t n)
{
    if (n == 0) /* undefined in OEIS; return 0 for safety */
        return (seq_t)0;

    if (!cache)          /* first call – set up initial terms */
        init_cache();

    if (n <= last)       /* already computed */
        return cache[n-1];

    grow_cache(n);       /* make sure there is room for all new terms */

    /* compute terms from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* indices are 1‑based: a(i-k) at cache[i-k-1] */
        seq_t a1 = cache[i-2]; /* a(i-1) */
        seq_t a2 = cache[i-3]; /* a(i-2) */
        seq_t a3 = cache[i-4]; /* a(i-3) */
        seq_t a4 = cache[i-5]; /* a(i-4) */
        seq_t a5 = cache[i-6]; /* a(i-5) */

        /* recurrence: 4*a1 -5*a2 + a3 + 2*a4 - a5 */
        seq_t term = 4 * a1 - 5 * a2 + a3 + 2 * a4 - a5;
        cache[i-1] = term;
    }
    last = n;
    return cache[n-1];
}
