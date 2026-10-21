/*
 * A221591  Number of 0..2 arrays of length n with each element differing from
 *          at least one neighbor by 1 or less.
 * https://oeis.org/A221591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + a(n-3) (for n > 4) Initial values:
 *            a(1)=0, a(2)=7, a(3)=17, a(4)=49 (OEIS A221591)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* 1‑indexed array storing sequence values */
static size_t cache_size = 0;   /* current allocated size (≥1) */

/* Expand cache to at least size+1 (1‑indexed) and compute missing values. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size) return;          /* already ready */

    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(a_cache,
                                        new_size * sizeof(seq_t));
    if (!new_cache) abort();                  /* allocation failure */

    /* compute missing values */
    for (size_t i = cache_size; i <= n; ++i) {
        if (i == 1)          new_cache[i] = (seq_t)0;
        else if (i == 2)     new_cache[i] = (seq_t)7;
        else if (i == 3)     new_cache[i] = (seq_t)17;
        else if (i == 4)     new_cache[i] = (seq_t)49;
        else
            new_cache[i] = ((seq_t)2 * new_cache[i-1] +
                            (seq_t)2 * new_cache[i-2] +
                            new_cache[i-3]);
    }

    a_cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1, return 0 */
    ensure_cache(n);
    return a_cache[n];
}
