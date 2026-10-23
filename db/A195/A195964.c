/*
 * A195964  Number of n X 3 0..4 arrays with each element equal to the number
 *          its horizontal and vertical neighbors equal to itself.
 * https://oeis.org/A195964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-3)+2*a(n-4)+2*a(n-5)+4*a(n-6)-a(n-9) for n>11 with
 *            initial values a(1)=2,a(2)=2,a(3)=5,a(4)=7,a(5)=14,a(6)=25,
 *            a(7)=35,a(8)=56,a(9)=107,a(10)=173,a(11)=284
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 42, u64 n <= 85, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 0‑based array: cache[0] = a(1) */
static size_t cache_cap = 0;         /* current capacity (in seq_t elements) */
static idx_t last_n_calculated = 0;  /* maximum n for which cache[n‑1] is set */

/* Ensure the cache has space for at least 'need' elements.  */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                /* out of memory – abort */
    cache = tmp;
    cache_cap = new_cap;
}

/* Hard‑coded first 11 terms. */
static const seq_t init[11] = {
    (seq_t)2, (seq_t)2, (seq_t)5, (seq_t)7, (seq_t)14,
    (seq_t)25, (seq_t)35, (seq_t)56, (seq_t)107, (seq_t)173, (seq_t)284
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* sequence is defined for n≥1 */

    ensure_cache((size_t)n);

    /* Initialize missing early terms (a(1)…a(11)). */
    if (last_n_calculated < 11) {
        size_t upto = n <= 11 ? (size_t)n : 11;
        for (size_t i = last_n_calculated; i < upto; ++i)
            cache[i] = init[i];
        last_n_calculated = (idx_t)upto;
        if (last_n_calculated >= n)   /* requested term was among the initial ones */
            return cache[n-1];
    }

    /* Compute remaining terms using the recurrence. */
    while (last_n_calculated < n) {
        idx_t k = last_n_calculated + 1;          /* current n */
        seq_t val =
            (seq_t)2 * cache[k-4] +  /* 2*a(k-3) */
            (seq_t)2 * cache[k-5] +  /* 2*a(k-4) */
            (seq_t)2 * cache[k-6] +  /* 2*a(k-5) */
            (seq_t)4 * cache[k-7] -  /* 4*a(k-6) */
            cache[k-10];             /* -a(k-9) */
        cache[k-1] = val;
        ++last_n_calculated;
    }

    return cache[n-1];
}
