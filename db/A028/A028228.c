/*
 * A028228  Expansion of 1/((1-7x)(1-10x)(1-11x)(1-12x)).
 * https://oeis.org/A028228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028228: Expansion of 1/((1-7x)(1-10x)(1-11x)(1-12x))
   Recurrence: a(n) = 40*a(n-1) - 593*a(n-2) + 3854*a(n-3) - 9240*a(n-4),
   with initial values: a(0)=1, a(1)=40, a(2)=1007, a(3)=20414. */
#include <stdlib.h>

/* seq_t and idx_t are defined by the build environment.  */
static seq_t *cache = NULL;      /* dynamically grown cache of sequence values */
static size_t cache_size = 0;    /* number of elements currently stored in cache */

/* Return a(n) for any n >= 0.  The function is memoized so any order of
   calls is correct and efficient. */
seq_t a(idx_t n)
{
    /* Handle the trivial cases directly. */
    if (n < 4) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)40;
            case 2: return (seq_t)1007;
            case 3: return (seq_t)20414;
        }
    }

    /* Initialise cache with the first four terms if not yet done. */
    if (!cache) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure: return 0 as a fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)40;
        cache[2] = (seq_t)1007;
        cache[3] = (seq_t)20414;
    }

    /* If required, extend and recompute up to n. */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* realloc failure: return 0 as a fallback */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            /* Compute the term using the recurrence.  All intermediate
               operations are performed in seq_t; the final result is
               guaranteed to be non‑negative for the sequence. */
            seq_t term = cache[i-1] * (seq_t)40
                       - cache[i-2] * (seq_t)593
                       + cache[i-3] * (seq_t)3854
                       - cache[i-4] * (seq_t)9240;
            cache[i] = term;
        }
        cache_size = new_size;
    }

    return cache[n];
}
