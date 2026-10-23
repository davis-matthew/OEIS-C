/*
 * A023392  a(n+1) = a(n) converted to base 10 from base 9 (written in base
 *          10).
 * https://oeis.org/A023392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 9; a(n+1) = the base‑9 digits of a(n) interpreted as a
 *            base‑10 integer
 * Range:     u8 n <= 32, u16 n <= 50, u32 n <= 66, u64 n <= 82, u128 n <= 97,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Convert a decimal integer `curr` to the next term by interpreting its base‑9
   digits as a decimal number.  Only integer arithmetic is used. */
static seq_t next_from(seq_t curr)
{
    seq_t next  = 0;
    seq_t pow10 = 1;
    while (curr > 0)
    {
        /* curr % 9 gives the least significant base‑9 digit */
        seq_t digit = curr % (seq_t)9;
        curr /= (seq_t)9;
        next += digit * pow10;
        if (curr > 0)
            pow10 *= (seq_t)10;
    }
    return next;
}

/* Compute the n‑th term of A023392.  Caching allows arbitrary order of
   calls without recomputing preceding values. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1 */

    static seq_t *cache      = NULL;
    static idx_t cache_max   = 0;

    /* Grow the cache if necessary. */
    if (n > cache_max)
    {
        seq_t *new_cache;
        if (cache == NULL)
            new_cache = seq_alloc(n);                 /* first allocation   */
        else
            new_cache = seq_realloc(cache, cache_max, n);   /* grow existing */

        if (!new_cache)
            return 0;                               /* allocation failed  */

        /* Compute new entries.  cache[0] corresponds to a(1). */
        for (idx_t i = cache_max; i < n; ++i)
        {
            if (i == 0)
                new_cache[i] = (seq_t)9;
            else
                new_cache[i] = next_from(new_cache[i - 1]);
        }

        cache      = new_cache;
        cache_max  = n;
    }

    return cache[n - 1];
}
