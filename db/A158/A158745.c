/*
 * A158745  a(3n)=A130750(n). a(3n+1)=A130752(n). a(3n+2)=A130755(n).
 * https://oeis.org/A158745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = 3*a(n-3)-3*a(n-6)+2*a(n-9) for n≥9; a(0)=1, a(1)=2,
 *            a(2)=3, a(3)=3, a(4)=5, a(5)=4, a(6)=8, a(7)=9, a(8)=7
 * Range:     u8 n <= 21, u16 n <= 44, u32 n <= 93, u64 n <= 188,
 *            u128 n <= 381, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;      /* array of computed a(n) */
static idx_t  max_cached = 0;         /* highest index that has a value */
static size_t cap        = 0;         /* allocated capacity */

static const seq_t base_vals[9] = { (seq_t)1, (seq_t)2, (seq_t)3,
                                   (seq_t)3, (seq_t)5, (seq_t)4,
                                   (seq_t)8, (seq_t)9, (seq_t)7 };

/* Ensure the cache is large enough to hold indices up to n (inclusive). */
static void ensure_cache(size_t n)
{
    if (n + 1 > cap)
    {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= n) new_cap <<= 1;
        /* C++ requires a cast from void* to seq_t* */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);    /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* For n < 9 just return the hard‑coded values. */
    if (n < 9) return base_vals[(size_t)n];

    /* If we are the first caller that needs n ≥ 9, initialise the
       cache with the first nine base values. */
    if (max_cached < 9)
    {
        ensure_cache(8);
        for (size_t i = 0; i < 9; ++i) cache[i] = base_vals[i];
        max_cached = 8;
    }

    /* If we already computed up to n, return the cached value. */
    if (n <= max_cached) return cache[(size_t)n];

    /* Compute values up to n, storing them in the cache. */
    ensure_cache((size_t)n);
    for (idx_t i = max_cached + 1; i <= n; ++i)
    {
        /* a(i) = 3*a(i-3) - 3*a(i-6) + 2*a(i-9)   (non‑negative intermediate) */
        seq_t t = 3 * cache[(size_t)(i - 3)];
        t -= 3 * cache[(size_t)(i - 6)];
        t += 2 * cache[(size_t)(i - 9)];
        cache[(size_t)i] = t;
    }
    max_cached = n;
    return cache[(size_t)n];
}
