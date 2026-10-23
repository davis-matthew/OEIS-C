/*
 * A033144  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,0.
 * https://oeis.org/A033144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*a(n-1) + a(n-3) - 8*a(n-4) with a(1)=1, a(2)=8,
 *            a(3)=64, a(4)=513. (Equivalently, a(n) = round((64/511) * 8^n).)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* cache of computed terms, indexed by n (1-based) */
static size_t  cap    = 0;      /* capacity of the cache array (in terms of n+1)  */
static size_t  len    = 0;      /* highest n value currently stored in cache       */

/* Ensure the cache can hold a term of index n. Dynamically grow the array if needed. */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n < cap)
        return;
    size_t new_cap = (cap == 0) ? 16 : cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cap   = new_cap;
}

/* Return the nth term of sequence A033144. */
seq_t
a(idx_t n)
{
    /* NOTE:  a(0) is outside the OEIS offset; behaviour is undefined but we return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Grow the cache if necessary. */
    ensure_cache(n);

    /* If already computed, return the stored value. */
    if (len >= (size_t)n)
        return cache[n];

    /* Compute successive terms up to n using the recurrence. */
    for (size_t i = len + 1; i <= (size_t)n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)8;
        else if (i == 3)
            cache[i] = (seq_t)64;
        else if (i == 4)
            cache[i] = (seq_t)513;
        else
            cache[i] = 8 * cache[i - 1] + cache[i - 3] - 8 * cache[i - 4];
    }

    len = (size_t)n;
    return cache[n];
}
