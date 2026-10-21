/*
 * A208600  Number of 6-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero.
 * https://oeis.org/A208600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 67, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A208600
   Recurrence: a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) - a(n-6)
   for n > 6.  Initial values (n = 1..6):
   a(1) = 26, a(2) = 297, a(3) = 1564, a(4) = 5457,
   a(5) = 14838, a(6) = 34153. */

#include <stdlib.h>
#include <stddef.h>

/* Cached sequence values – 1‑based indexing, index 0 unused. */
static seq_t *seq_cache  = NULL;   /* array of length cache_size */
static idx_t  cache_size = 0;      /* number of allocated entries */
static idx_t  last_used  = 0;      /* largest index that has been computed */

/* Ensure the cache can accommodate indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Need at least n+1 entries (index 0..n). */
    if (cache_size > n)
        return;

    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, (size_t)new_size * sizeof(seq_t));
    if (!new_cache)            /* allocation failed – unlikely for n<=10000 */
        return;                /* let caller deal with failure */

    /* Zero any newly allocated positions (they are uninitialized). */
    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = (seq_t)0;

    seq_cache  = new_cache;
    cache_size = new_size;
}

/* Return the n-th term of A208600 (1‑based).  a(0) is undefined. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* safety: out of range */

    /* Ensure we have space for at least the first six terms. */
    if (last_used < 6)
        ensure_cache(6);

    /* Initialize the first six terms once. */
    if (last_used < 6) {
        static const seq_t init_vals[6] = {26, 297, 1564, 5457, 14838, 34153};
        for (idx_t i = 1; i <= 6; ++i)
            seq_cache[i] = init_vals[i - 1];
        last_used = 6;
    }

    /* If we already have the requested value, return it. */
    if (n <= last_used)
        return seq_cache[n];

    /* Allocate space for the new terms. */
    ensure_cache(n);

    /* Compute terms up to n using the recurrence. */
    for (idx_t i = last_used + 1; i <= n; ++i) {
        seq_t term =   (seq_t)6  * seq_cache[i - 1]
                    - (seq_t)15 * seq_cache[i - 2]
                    + (seq_t)20 * seq_cache[i - 3]
                    - (seq_t)15 * seq_cache[i - 4]
                    + (seq_t)6  * seq_cache[i - 5]
                    -          seq_cache[i - 6];
        seq_cache[i] = term;
    }
    last_used = n;
    return seq_cache[n];
}
