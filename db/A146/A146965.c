/*
 * A146965  a(n) = 10*a(n-1) - 18*a(n-2) with a(0)=1, a(1)=5.
 * https://oeis.org/A146965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   allocate_from, deallocate_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A146965
   Recurrence: a(n) = 10*a(n-1) - 18*a(n-2)
   Initial values: a(0) = 1, a(1) = 5
   a(0) is the first term listed. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/allocate_from.h"
#include "../utils/deallocate_seq.h"

/* ------------------------------------------------------------
   Computes a(n), any n >= 0.
   Memoises all values up to the largest n requested so far.
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* values a(i), i < cap */
    static size_t cap   = 0;           /* number of cached values */

    /* initialise base terms if nothing cached yet */
    if (cap == 0)
    {
        cache = allocate_from(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cap = 2;
        if (!cache) return (seq_t)0;   /* allocation failed (unlikely) */
    }

    /* expand cache if necessary */
    if ((size_t)n + 1 > cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = allocate_from(new_cap);
        if (!new_cache) return (seq_t)0; /* allocation failed */

        /* copy old values */
        for (size_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];

        /* compute missing entries using the recurrence */
        for (size_t i = cap; i < new_cap; ++i)
        {
            /* use explicit casts to seq_t for constants */
            new_cache[i] = (seq_t)10 * new_cache[i - 1] - (seq_t)18 * new_cache[i - 2];
        }

        deallocate_seq(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    return cache[(size_t)n];
}
