/*
 * A220616  Number of ways to reciprocally link elements of an n X 3 array
 *          either to themselves or to exactly one horizontal, diagonal and
 *          antidiagonal neighbor.
 * https://oeis.org/A220616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 4*a(n-2) - 10*a(n-3) + 4*a(n-4) (n ≥ 5)
 *            Initial values: a(1)=3, a(2)=21, a(3)=125, a(4)=765
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached values of the sequence.  Uses 1‑based indexing; element 0 is unused. */
static seq_t *cache = NULL;       /* 1‑based indexing: cache[0] unused   */
static idx_t  cache_cap = 0;      /* current capacity (including unused index) */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n)
        return;

    /* Allocate space for indices 0..n inclusive. */
    idx_t new_cap = n + 1;   /* +1 to keep index 0 unused */
    /* realloc works in both C and C++ when the result is cast appropriately. */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!new_cache)          /* out‑of‑memory */
        exit(1);

    /* Initialise any newly allocated elements to 0.  The sequence never
       contains 0, so a 0 value means "not yet computed". */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;     /* behaviour below the offset is irrelevant */

    ensure_cache(n);

    /* Compute values up to n on demand. */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (cache[i] != (seq_t)0)          /* already known */
            continue;

        switch (i)
        {
        case 1: cache[1] = (seq_t)3;         break;
        case 2: cache[2] = (seq_t)21;        break;
        case 3: cache[3] = (seq_t)125;       break;
        case 4: cache[4] = (seq_t)765;       break;
        default:
            /* Use the linear recurrence: a(n) = 7*a(n-1) - 4*a(n-2) - 10*a(n-3) + 4*a(n-4) */
            cache[i] = cache[i-1] * (seq_t)7
                       - cache[i-2] * (seq_t)4
                       - cache[i-3] * (seq_t)10
                       + cache[i-4] * (seq_t)4;
            break;
        }
    }

    return cache[n];
}
