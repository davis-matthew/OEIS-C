/*
 * A028044  Expansion of 1/((1-3x)(1-4x)(1-8x)(1-10x)).
 * https://oeis.org/A028044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10^(n+4)-21*8^(n+3)+35*4^(n+3)-8*3^(n+4))/840
 *            Recurrence: a(0)=1, a(1)=25, a(2)=407, a(3)=5501 a(n) =
 *            25*a(n-1) - 218*a(n-2) + 776*a(n-3) - 960*a(n-4) (n>=4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches all values up to the largest n requested.
   It works with any seq_t that supports +, -, *, / and assignment.   */

#include <stdlib.h>   /* for malloc, realloc */

static seq_t *seq_cache = NULL;     /* cached values */
static idx_t seq_cache_size = 0;    /* number of elements currently cached */

/* Ensure that seq_cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (seq_cache_size > n) return;          /* already sufficient */

    /* Grow the cache to hold indices 0..n.  Use realloc to preserve existing data. */
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_cache) {                        /* allocation failed */
        /* In a real program we might handle this differently; here we simply exit. */
        /* exit is allowed in C/C++ for fatal errors. */
        exit(1);
    }
    seq_cache = new_cache;

    /* If this is the first allocation, initialise the first four terms. */
    if (seq_cache_size == 0) {
        seq_cache[0] = (seq_t)1;
        if (new_cap > 1) seq_cache[1] = (seq_t)25;
        if (new_cap > 2) seq_cache[2] = (seq_t)407;
        if (new_cap > 3) seq_cache[3] = (seq_t)5501;
    } else {
        /* If the cache was previously not long enough to hold the initial terms,
           ensure they are set. */
        if (new_cap > 1 && seq_cache_size <= 1) seq_cache[1] = (seq_t)25;
        if (new_cap > 2 && seq_cache_size <= 2) seq_cache[2] = (seq_t)407;
        if (new_cap > 3 && seq_cache_size <= 3) seq_cache[3] = (seq_t)5501;
    }

    /* Compute any new terms that were not present before.  Start from the first
       index that needs computation. */
    idx_t start = (seq_cache_size > 4) ? seq_cache_size : 4;
    for (idx_t i = start; i <= n; ++i) {
        /* Use explicit casts to seq_t for the constants to keep the expression
           compatible with all seq_t types. */
        seq_cache[i] = (seq_t)25  * seq_cache[i-1]
                     - (seq_t)218 * seq_cache[i-2]
                     + (seq_t)776 * seq_cache[i-3]
                     - (seq_t)960 * seq_cache[i-4];
    }

    seq_cache_size = new_cap;   /* update the cached size */
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
