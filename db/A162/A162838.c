/*
 * A162838  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A162838: number of reduced words of length n in
 * a Coxeter group with 34 generators S_i, relations S_i^2 = I,
 * (S_i S_j)^3 = I for all i ≠ j.
 *
 * The ordinary generating function is
 *   (t^3 + 2*t^2 + 2*t + 1) / (528*t^3 - 32*t^2 - 32*t + 1).
 *
 * From this we obtain the linear recurrence
 *   a(n) = 32*a(n-1) + 32*a(n-2) - 528*a(n-3)   for n ≥ 4,
 * with initial values
 *   a(0)=1, a(1)=34, a(2)=1122, a(3)=36465.
 *
 * The implementation memoises already computed terms in a static
 * array so that subsequent calls are O(1) after the first
 * expansion to the required index.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- static memoisation array ---------- */
    static seq_t *cache = NULL;          /* dynamic array of sequence values */
    static size_t cache_cap = 0;         /* current capacity of cache */
    static size_t max_computed = (size_t)-1; /* index of last computed term */
    static const seq_t init_vals[4] = {
        (seq_t)1,      /* a(0) */
        (seq_t)34,     /* a(1) */
        (seq_t)1122,   /* a(2) */
        (seq_t)36465   /* a(3) */
    };

    /* ---------- initialise memoisation array ---------- */
    if (cache_cap == 0) {
        /* Allocate space for at least the first four values */
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(*cache));
        if (!cache) return (seq_t)0; /* allocation failed, give 0 as fallback */
        for (size_t i = 0; i < 4; ++i) cache[i] = init_vals[i];
        max_computed = 3;
    }

    /* ---------- fast return for small indices ---------- */
    if ((size_t)n <= max_computed) return cache[(size_t)n];

    /* ---------- expand cache up to index n ---------- */
    size_t need = (size_t)n + 1;                /* required capacity */
    if (need > cache_cap) {                     /* grow array */
        size_t new_cap = cache_cap;
        while (new_cap < need) new_cap <<= 1;    /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
        if (!tmp) return (seq_t)0;               /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ---------- compute new terms ---------- */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = 32*a(i-1) + 32*a(i-2) - 528*a(i-3) */
        seq_t term = ((seq_t)32) * cache[i - 1]
                   + ((seq_t)32) * cache[i - 2]
                   - ((seq_t)528) * cache[i - 3];
        cache[i] = term;
    }
    max_computed = (size_t)n;
    return cache[(size_t)n];
}
