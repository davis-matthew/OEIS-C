/*
 * A223364  6X6X6 triangular graph coloring a rectangular array: number of nX2
 *          0..20 arrays where 0..20 label nodes of the fully triangulated
 *          graph and every array movement to a horizontal or vertical
 *          neighbor moves along an edge of this graph.
 * https://oeis.org/A223364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A223364
 * Recurrence: a(n) = 17*a(n-1) - 44*a(n-2) - 208*a(n-3) + 363*a(n-4)
 *                + 659*a(n-5) - 448*a(n-6) - 364*a(n-7) + 66*a(n-8) + 24*a(n-9)
 * Initial values (1‑based indexing):
 *   a(1)=90, a(2)=990, a(3)=11610, a(4)=139158, a(5)=1686042,
 *   a(6)=20537766, a(7)=250834914, a(8)=3067525350, a(9)=37537522362
 * A memoised iterative implementation is provided below.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t max_computed = 0;

static const seq_t init_vals[9] = {
    90ULL,          /* a(1) */
    990ULL,         /* a(2) */
    11610ULL,       /* a(3) */
    139158ULL,      /* a(4) */
    1686042ULL,     /* a(5) */
    20537766ULL,    /* a(6) */
    250834914ULL,   /* a(7) */
    3067525350ULL,  /* a(8) */
    37537522362ULL  /* a(9) */
};

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined below offset */

    if (cache == NULL) {
        /* Allocate initial capacity for indices 0..9 (10 slots) */
        cap = 16; /* slightly more than needed for small n */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure */
        cache[0] = 0; /* unused */
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = init_vals[i - 1];
        max_computed = 9;
    }

    /* Ensure buffer is large enough for indices up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t pos = (seq_t)17 * cache[i - 1]
                  + (seq_t)363 * cache[i - 4]
                  + (seq_t)659 * cache[i - 5]
                  + (seq_t)66  * cache[i - 8]
                  + (seq_t)24  * cache[i - 9];

        seq_t neg = (seq_t)44  * cache[i - 2]
                  + (seq_t)208 * cache[i - 3]
                  + (seq_t)448 * cache[i - 6]
                  + (seq_t)364 * cache[i - 7];

        cache[i] = pos - neg;
    }
    if (n > max_computed) max_computed = n;

    return cache[n];
}
