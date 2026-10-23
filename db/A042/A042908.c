/*
 * A042908  Numerators of continued fraction convergents to sqrt(986).
 * https://oeis.org/A042908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 314*a(n-3) + a(n-6) (for n ≥ 6) Initial values: a(0)=31,
 *            a(1)=63, a(2)=157, a(3)=9797, a(4)=19751, a(5)=49299 (recurrence
 *            derivable from the generating function)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*---------------------------------------------------------------------------*/
/*  Cached storage of already computed terms                               */
/*---------------------------------------------------------------------------*/
static seq_t *cache      = NULL;   /* array holding a(0)..a(max_computed)      */
static size_t cache_cap  = 0;      /* current allocated capacity (in elems)    */
static idx_t  max_computed = (idx_t)-1; /* index of largest computed term */

/* The six seed values (the initial terms of the sequence)                 */
static const seq_t init[6] = {
    (seq_t)31,
    (seq_t)63,
    (seq_t)157,
    (seq_t)9797,
    (seq_t)19751,
    (seq_t)49299
};

/*---------------------------------------------------------------------------*/
/*   Ensure that cache is allocated and contains a(0)..a(n) if it is computable
 *   from the recurrence.                                                    */
/*---------------------------------------------------------------------------*/
static void ensure_cache(idx_t n)
{
    /* If nothing has been computed yet, allocate and seed with the first
     * six values.  Then compute from the current maximum index up to n. */
    if (max_computed == (idx_t)-1) {
        /* allocate initial capacity (at least 16) */
        size_t cap = 16;
        while (cap <= (size_t)n + 6) cap <<= 1;  /* ensure room for the needed indices */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) abort();                     /* out of memory */

        /* copy the six seed values */
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init[i];

        cache_cap = cap;
        max_computed = 5;
    }

    /* Expand the cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* For i < 6 we have already stored the seed values directly.
           The loop only runs for i >= 6. The recurrence uses a(i-3)
           and a(i-6), which are guaranteed to be available because
           we compute in increasing order. */
        cache[i] = (seq_t)( (seq_t)314 * cache[i - 3] + cache[i - 6] );
    }

    if (n > max_computed)
        max_computed = n;
}

/*---------------------------------------------------------------------------*/
/*   Public sequence function                                               */
/*---------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* For n < 6 simply return the pre‑defined seed value */
    if (n < 6)
        return init[n];

    /* Otherwise, make sure the desired term is computed and cached.   */
    ensure_cache(n);
    return cache[n];
}
