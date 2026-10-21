/*
 * A229455  Number of defective 3-colorings of an n X 4 0..2 array connected
 *          horizontally and vertically with exactly one mistake and colors
 *          introduced in row-major 0..2 order.
 * https://oeis.org/A229455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) - 111*a(n-2) + 282*a(n-3) - 333*a(n-4) +
 *            180*a(n-5) - 36*a(n-6) (empirical recurrence) Initial values:
 *            a(1)=6, a(2)=84, a(3)=863, a(4)=7940, a(5)=68790, a(6)=573342,
 *            a(7)=4651079
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Cache for already computed values */
#ifdef __cplusplus
/* In C++ use a vector to store arbitrary‑precision objects. */
static std::vector<seq_t> cache_vec;
static idx_t max_computed_vec = 0;
#else
/* In C use raw memory allocation; seq_t is a plain arithmetic type. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_computed = 0;
#endif

/* Initial terms (index 0 unused).  The literals are small enough for all
   supported seq_t types; a conversion will construct the appropriate
   arbitrary‑precision value in C++. */
static const seq_t init_vals[8] = {
    0,   /* a(0) – unused, kept for 1‑based indexing */
    6, 84, 863, 7940, 68790, 573342, 4651079
};

/* Helper macro to grow the cache (only needed in C compilation). */
#ifndef __cplusplus
static void ensure_cache_size(idx_t needed) {
    if (cache_cap <= (size_t)needed) {
        size_t new_cap = (size_t)needed + 1;      /* +1 for 1‑based index */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {                         /* allocation failed */
            exit(1);                               /* terminate program */
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}
#endif

/* Main function implementing the sequence.  It may be called in any order
   and uses memoisation to avoid recomputation. */
seq_t a(idx_t n)
{
    /* Accept only positive indices; behaviour for n < 1 is unspecified. */
    if (n <= 0) return 0;

    /* ---- Handle initial values directly ---- */
    if (n < 8) return init_vals[n];

#ifdef __cplusplus
    /* ---- C++ path: use std::vector ---- */
    /* Grow vector if necessary */
    if (cache_vec.size() <= (size_t)n)
        cache_vec.resize((size_t)n + 1);           /* +1 for 1‑based */

    /* Initialise cache up to the 7th term once */
    if (max_computed_vec < 7) {
        for (idx_t i = max_computed_vec + 1; i <= 7; ++i)
            cache_vec[(size_t)i] = init_vals[(size_t)i];
        max_computed_vec = 7;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed_vec + 1; i <= n; ++i) {
        seq_t t1 = cache_vec[(size_t)(i - 1)];
        seq_t t2 = cache_vec[(size_t)(i - 2)];
        seq_t t3 = cache_vec[(size_t)(i - 3)];
        seq_t t4 = cache_vec[(size_t)(i - 4)];
        seq_t t5 = cache_vec[(size_t)(i - 5)];
        seq_t t6 = cache_vec[(size_t)(i - 6)];
        cache_vec[(size_t)i] = 18 * t1
                             - 111 * t2
                             + 282 * t3
                             - 333 * t4
                             + 180 * t5
                             - 36 * t6;
    }
    max_computed_vec = n;
    return cache_vec[(size_t)n];
#else
    /* ---- C path: use malloc/realloc ---- */
    ensure_cache_size(n);

    /* Initialise cache up to the 7th term once */
    if (max_computed < 7) {
        for (idx_t i = max_computed + 1; i <= 7; ++i)
            cache[(size_t)i] = init_vals[(size_t)i];
        max_computed = 7;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t t1 = cache[(size_t)(i - 1)];
        seq_t t2 = cache[(size_t)(i - 2)];
        seq_t t3 = cache[(size_t)(i - 3)];
        seq_t t4 = cache[(size_t)(i - 4)];
        seq_t t5 = cache[(size_t)(i - 5)];
        seq_t t6 = cache[(size_t)(i - 6)];
        cache[(size_t)i] = 18 * t1
                         - 111 * t2
                         + 282 * t3
                         - 333 * t4
                         + 180 * t5
                         - 36 * t6;
    }
    max_computed = n;
    return cache[(size_t)n];
#endif
}
