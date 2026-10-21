/*
 * A231310  Number of (n+1) X (1+1) 0..2 arrays with no element equal to a
 *          strict majority of its horizontal, vertical, diagonal and diagonal
 *          neighbors, with values 0..2 introduced in row major order.
 * https://oeis.org/A231310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 2*a(n-2) - 24*a(n-3) - 40*a(n-4) + 16*a(n-5);
 *            initial values: a(1)=9, a(2)=57, a(3)=414, a(4)=2942, a(5)=20996
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* internal cache for memoisation */
static seq_t *cache = NULL;        /* array of computed values, indexed 0..n */
static size_t cache_capacity = 0;  /* allocated size */
static idx_t  max_computed = 0;    /* highest index whose value is known */

/* Ensure that the cache can store indices up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if (cache_capacity > (size_t)n)
        return;  /* already large enough */

    size_t new_capacity = cache_capacity ? cache_capacity * 2 : 16;
    while (new_capacity <= (size_t)n)
        new_capacity <<= 1;                     /* double until > n */

    seq_t *new_cache = (seq_t *)malloc(new_capacity * sizeof(seq_t));
    if (!new_cache) {
        /* If allocation fails, abort – this sequence is only used in
           controlled test environments where a full system allocation
           failure is unlikely. */
        abort();
    }

    /* copy existing values */
    for (size_t i = 0; i < cache_capacity; ++i)
        new_cache[i] = cache[i];

    free(cache);
    cache = new_cache;
    cache_capacity = new_capacity;
}

/* Initialise the first five terms of the sequence. */
static void
init_cache(void)
{
    ensure_cache(5);          /* need indices up to 5 */
    cache[1] = (seq_t)9;
    cache[2] = (seq_t)57;
    cache[3] = (seq_t)414;
    cache[4] = (seq_t)2942;
    cache[5] = (seq_t)20996;
    max_computed = 5;
}

/* Return the n-th term of sequence A231310 for n ≥ 1. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* not part of the listed offset, but harmless */

    if (!cache)
        init_cache();

    /* If the requested value is already known, return it.  */
    if (n <= max_computed)
        return cache[n];

    /* Compute up to n using the recurrence relation. */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        /* Use an addition‑then‑subtraction sequence to avoid unsigned
           underflow.   */
        seq_t val = (seq_t)8  * cache[i - 1];
        val += (seq_t)16 * cache[i - 5];

        seq_t sub = (seq_t)2  * cache[i - 2];
        sub += (seq_t)24 * cache[i - 3];
        sub += (seq_t)40 * cache[i - 4];

        val -= sub;

        ensure_cache(i);
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
