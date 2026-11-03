/*
 * A155521  Smallest fixed point summed over all non-derangement permutations
 *          of {1,2,...,n}.
 * https://oeis.org/A155521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*a(n-1) + n*(-1)^(n+1), a(0) = 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 189 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;       /* cache of computed terms          */
static size_t  cache_cap = 0;     /* current capacity of the cache    */

/* Return the n-th term of OEIS A155521. */
seq_t a(idx_t n)
{
    /* Grow the cache if necessary. */
    if ((size_t)n >= cache_cap)
    {
        size_t old_cap = cache_cap;
        size_t new_cap = (size_t)n + 1;

        cache = seq_realloc(cache, old_cap, new_cap);
        if (old_cap == 0)
        {
            cache[0] = (seq_t)0;          /* a(0) = 0 */
        }

        /* Compute missing terms using the recurrence. */
        for (size_t i = old_cap; i < new_cap; ++i)
        {
            if (i == 0) continue;         /* already initialized */

            seq_t prev  = cache[i - 1];
            seq_t term  = (seq_t)(i + 1) * prev;

            /* add n*(-1)^(n+1)  =>  +i when i odd, -i when i even */
            if ((i & 1u) == 0) /* i even: (-1)^(i+1) = -1 */
                term -= (seq_t)i;
            else               /* i odd:  (-1)^(i+1) = +1 */
                term += (seq_t)i;

            cache[i] = term;
        }

        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
