/*
 * A126943  The sequence d[n] defined in A126939.
 * https://oeis.org/A126943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=4, a(3)=14, a(4)=36, a(5)=116 a(n) =
 *            2*a(n‑1) + a(n‑2) + 7*a(n‑3) + 3*a(n‑4) + 3*a(n‑5) + a(n‑6) for
 *            n ≥ 6
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for previously computed values. */
static seq_t *cache      = NULL;   /* pointer to array of seq_t */
static size_t cache_len  = 0;      /* number of valid elements */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Initial values of the sequence. */
static const seq_t init_vals[6] = {
    (seq_t)0,
    (seq_t)0,
    (seq_t)4,
    (seq_t)14,
    (seq_t)36,
    (seq_t)116
};

/* Grow the cache capacity to at least `need`.  Uses realloc for
   simplicity and correctness in both C and C++ environments. */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need) return;

    /* Minimum growth factor is 2, but make sure we at least allocate `need`. */
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
    /* If realloc fails, the old block remains intact; the function will
       crash on subsequent accesses, which is acceptable for this exercise. */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n).  The function is safe to call in any order and
   memoizes results in a static cache. */
seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n < 6) return init_vals[(size_t)n];

    /* First call: allocate cache and copy initial values. */
    if (cache == NULL) {
        /* Allocate enough space for at least the requested n. */
        size_t need = (size_t)n + 1;
        if (cache_cap < need) ensure_capacity(need);
        /* Copy initial values. */
        for (size_t i = 0; i < 6; ++i) cache[i] = init_vals[i];
        cache_len = 6;
    }
    /* If the cache has been partially filled (because a smaller n was
       requested earlier), fill it up to the initial values. */
    else if (cache_len < 6) {
        for (size_t i = cache_len; i < 6; ++i) cache[i] = init_vals[i];
        cache_len = 6;
    }

    /* Expand the cache up to n, computing new values as needed. */
    while (cache_len <= (size_t)n) {
        /* Ensure we have space for the next element. */
        if (cache_len == cache_cap) ensure_capacity(cache_cap << 1);

        /* Indices to the last six values are always valid because
           cache_len >= 6 here. */
        seq_t v0 = cache[cache_len - 1];          /* a(n-1) */
        seq_t v1 = cache[cache_len - 2];          /* a(n-2) */
        seq_t v2 = cache[cache_len - 3];          /* a(n-3) */
        seq_t v3 = cache[cache_len - 4];          /* a(n-4) */
        seq_t v4 = cache[cache_len - 5];          /* a(n-5) */
        seq_t v5 = cache[cache_len - 6];          /* a(n-6) */

        cache[cache_len] = v0 * (seq_t)2
                          + v1
                          + v2 * (seq_t)7
                          + v3 * (seq_t)3
                          + v4 * (seq_t)3
                          + v5;
        ++cache_len;
    }

    return cache[(size_t)n];
}
