/*
 * A120747  Sequence relating to the 11-gon (or hendecagon).
 * https://oeis.org/A120747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 72,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A120747
   a(1) = 0
   a(2) = 1
   a(3) = 4
   a(4) = 14
   a(5) = 50
   a(n) = 3*a(n-1)+3*a(n-2)-4*a(n-3)-a(n-4)+a(n-5)   for n ≥ 6 */

/*    The function uses an internal cache so that calls in any order
    are fast.  It works for any seq_t type that supports ordinary
    integer arithmetic (including arbitrary‑precision builtins).  */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_seq.h"

/*  Computes a(idx_t n) */
seq_t a(idx_t n)
{
    /* Sequence is defined only for n ≥ 1 */
    if (n < 1)
        return 0;

    /* Handle small n directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)14;
    if (n == 5) return (seq_t)50;

    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;   /* cache[i] holds a(i) for i ≥ 1 */
    static idx_t  last  = 0;      /* highest index we have computed */
    static size_t cap   = 0;      /* allocated array size (in seq_t) */

    /* Ensure cache big enough for index n */
    if (cap <= n)
    {
        size_t new_cap = (size_t)n + 2;  /* a little spare space */
        seq_t *new_cache = alloc_seq(new_cap);
        if (!new_cache)
            exit(1);                 /* allocation error */

        if (cache)
        {
            /* Copy existing values */
            for (idx_t i = 0; i <= last; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* If we haven't initialized the first five terms yet, do it now */
    if (last < 5)
    {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)14;
        cache[5] = (seq_t)50;
        last = 5;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        cache[i] = (seq_t)(
                        (seq_t)3 * cache[i-1] +
                        (seq_t)3 * cache[i-2] -
                        (seq_t)4 * cache[i-3] -
                        (seq_t)1 * cache[i-4] +
                        (seq_t)1 * cache[i-5]
                    );
    }

    last = n;
    return cache[n];
}
