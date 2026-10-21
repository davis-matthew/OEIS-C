/*
 * A233838  Number of (n+2) X (1+2) 0..3 arrays with no increasing sequence of
 *          length 3 horizontally or antidiagonally downwards.
 * https://oeis.org/A233838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 56*a(n-1) + 16*a(n-2) - 768*a(n-3) + 512*a(n-4) with
 *            a(1)=201664, a(2)=11305216, a(3)=633584640, a(4)=35508588544
 *            (OEIS A233838)
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 8, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort, size_t */

static seq_t *cache   = NULL;   /* 1‑based cache, index 0 unused */
static size_t cap     = 0;      /* current allocation size (0 means empty) */
static size_t last_filled = 0;  /* highest index that has been computed */

/* Ensure that cache can store values up to index n (inclusive). */
static void grow_cache(idx_t n)
{
    if (cap > 0 && (size_t)n < cap)
        return;                       /* already large enough */

    size_t new_cap = cap ? cap : 4;     /* start with 4 */
    while (new_cap <= (size_t)n)
        new_cap <<= 1;                  /* double until big enough */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)                      /* allocation failed */
        abort();

    cache = new_cache;
    cap   = new_cap;
}

/* Return the n-th term of OEIS A233838.  n>=1 */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1, return 0 (anything is fine) */
        return (seq_t)0;

    grow_cache(n);

    for (size_t i = last_filled + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)201664ULL;
        } else if (i == 2) {
            cache[2] = (seq_t)11305216ULL;
        } else if (i == 3) {
            cache[3] = (seq_t)633584640ULL;
        } else if (i == 4) {
            cache[4] = (seq_t)35508588544ULL;
        } else {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            seq_t t4 = cache[i - 4];
            /* a(n) = 56*a(n-1) + 16*a(n-2) - 768*a(n-3) + 512*a(n-4) */
            seq_t val = (seq_t)56 * t1 + (seq_t)16 * t2
                        - (seq_t)768 * t3 + (seq_t)512 * t4;
            cache[i] = val;
        }
    }
    last_filled = (size_t)n;
    return cache[(size_t)n];
}
