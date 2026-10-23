/*
 * A154638  a(n) is the number of distinct reduced words of length n in the
 *          Coxeter group of "Apollonian reflections" in three dimensions.
 * https://oeis.org/A154638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct reduced words of length n in the
 *            Coxeter group of Apollonian reflections (three dimensions).
 *            Recurrence: a(0)=1, a(1)=5, a(2)=20, a(3)=70, a(n+3) = 3*a(n+2)
 *            + 3*a(n+1) - 6*a(n) for n ≥ 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation (external linkage for a only) */
static seq_t *cache = NULL;      /* array holding computed terms */
static idx_t cache_len = 0;      /* number of valid entries in cache */
static size_t cache_cap = 0;     /* current allocated size of cache */

/* Ensure the cache can store indices up to 'need' inclusively */
static void ensure_capacity(idx_t need)
{
    size_t required = (size_t)(need + 1);
    if (required <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < required)
        new_cap *= 2;

    void *tmp = realloc(cache, new_cap * sizeof(seq_t));
    cache = (seq_t *)tmp;                /* cast to seq_t* for C/C++ */
    cache_cap = new_cap;
}

/* Return the N‑th term of A154638 */
seq_t a(idx_t n)
{
    /* If the value is already cached, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* Initialise the first four terms lazily on first use */
    if (cache_len == 0) {
        ensure_capacity(3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)20;
        cache[3] = (seq_t)70;
        cache_len = 4;
    }

    /* If after initialization the requested index is smaller than
       cache_len, we can return the value directly. */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache to cover the requested index */
    ensure_capacity(n);
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        /* a(i) = 3*a(i-1) + 3*a(i-2) - 6*a(i-3) */
        cache[i] = ((seq_t)3 * a1) + ((seq_t)3 * a2) - ((seq_t)6 * a3);
    }
    cache_len = n + 1;
    return cache[n];
}
