/*
 * A070168  Irregular triangle of Terras-modified Collatz problem.
 * https://oeis.org/A070168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 248, u16 n <= 7727, u32 n <= 7727, u64 n <= 7727,
 *            u128 n <= 7727, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A070168: a(n) is the n‑th entry obtained by writing the rows of
 * the irregular triangle of the Terras‑modified Collatz map T(n)=n/2 if n is
 * even and T(n)=(3n+1)/2 otherwise, row by row.  The initial row consists
 * solely of 1; each subsequent row begins with its row index and continues
 * with the iterates of that index under T until the value 1 is reached. */

#include <stdlib.h>

/* Cache for the already computed terms */
static seq_t *cache = NULL;       /* pointer to cached values      */
static idx_t  cache_len = 0;      /* number of cached values       */
static idx_t  cache_cap = 0;      /* allocated capacity of cache   */
static idx_t  next_row  = 1;      /* the next row number to generate */

/* Grow the cache so that it can contain at least 'min_cap' elements. */
static void grow_cache(idx_t min_cap) {
    if (min_cap <= cache_cap) return;
    if (cache_cap == 0) cache_cap = 128;
    while (cache_cap < min_cap) cache_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    cache = tmp;
}

/* Generate rows until we have at least 'up_to' entries in the cache. */
static void extend_cache(idx_t up_to) {
    while (cache_len < up_to) {
        /* generate the next row (row number = next_row) */
        uint64_t val = (uint64_t)next_row;          /* current value in the row */
        if (cache_len + 1 > cache_cap) grow_cache(cache_len + 1);
        cache[cache_len++] = (seq_t)val;           /* store the first element */

        /* iterate until the value 1 is produced */
        while (val != 1ULL) {
            if ((val & 1ULL) == 0ULL)               /* even case */
                val >>= 1;
            else                                   /* odd case */
                val = (val * 3ULL + 1ULL) >> 1;     /* (3*val+1)/2 */

            if (cache_len + 1 > cache_cap) grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)val;       /* store the next element */
        }
        ++next_row;                                /* advance to next row */
    }
}

/* Public function required by the harness. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but harmless */
    if (cache == NULL) {           /* initialise cache once */
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    extend_cache(n);                /* ensure the n-th term is known */
    return cache[n - 1];            /* array is 0‑based, sequence is 1‑based */
}
