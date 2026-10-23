/*
 * A041431  Denominators of continued fraction convergents to sqrt(231).
 * https://oeis.org/A041431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 152*a(n-2) - a(n-4), with a(0)=1, a(1)=5, a(2)=151,
 *            a(3)=760
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;
static idx_t  cap     = 0;          /* allocated size of cache */
static idx_t  last    = (idx_t)-1;  /* last index that has a valid term */

/* Ensure the cache can hold indices 0..n (i.e. at least n+1 elements) */
static void ensure_capacity(idx_t n)
{
    if (cap > n) return;
    idx_t new_cap = cap ? cap * 2 : 4;
    while (new_cap <= n) new_cap <<= 1;          /* double until enough */

    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) abort();                          /* allocation failed */
    cache = tmp;
    cap   = new_cap;
}

/* Initialise the first four terms of the sequence.  This is called once. */
static void init_cache(void)
{
    if (last != (idx_t)-1) return;              /* already initialised */

    ensure_capacity(3);                        /* we need indices 0..3 */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)5;
    cache[2] = (seq_t)151;
    cache[3] = (seq_t)760;
    last    = 3;
}

seq_t a(idx_t n)
{
    init_cache();                               /* set the first four terms */

    if (n <= last) return cache[n];             /* already computed */

    /* Make sure the cache can hold up to index n */
    ensure_capacity(n);

    /* Compute the sequence from last+1 up to n, extending the cache as needed. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* The recurrence uses values that are already computed: a(i-2) and a(i-4). */
        cache[i] = (seq_t)(152 * cache[i - 2] - cache[i - 4]);
    }
    last = n;                                    /* update the last computed index */
    return cache[n];
}
