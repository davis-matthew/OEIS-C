/*
 * A087849  a(n) = a(a(a(n-1))) + abs(2*a(n-1)-n) starting with 1,1.
 * https://oeis.org/A087849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087849
   Formula: a(1)=1, a(2)=1,
            for n≥3: a(n)=a(a(a(n-1))) + abs(2*a(n-1)-n) */

#include <stdlib.h>

/* Cache of already computed terms.  The pointer type is seq_t* so it can point
   to integers or to a big‑integer class.  The value is a plain C integer
   representing an index into the cache (idx_t). */
static seq_t *cache = NULL;     /* array of computed values */
static idx_t  cache_cap = 0;    /* allocated capacity (in seq_t elements) */
static idx_t  computed   = 0;   /* highest index that has been computed */

/* Allocate/expand the cache so that it can hold at least indices up to n
   (inclusive).  The capacity is rounded up to the next power of two.  The
   function works in both C and C++ because the returned void* is explicitly
   cast to seq_t*. */
static void
ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* If realloc returns a non‑NULL pointer we replace the old block. */
    if (tmp == NULL) {
        /* In a real program we would handle allocation failure.
           Here we just abort to keep the code minimal. */
        abort();
    }
    /* Initialise any newly allocated part to zero: memory beyond the old
       capacity is uninitialised by realloc but the algorithm never reads
       it until we write to it.  The code below only writes into the range
       [computed+1 .. n], so we don't need explicit zeroing. */

    cache = tmp;
    cache_cap = new_cap;
}

/* Public entry point for the sequence.  The function works for any order of
   calls; it memoises results in a static array. */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined; guarding against underflow */
        return 0;

    /* Initialise the cache on the first call. */
    if (cache == NULL) {
        /* Allocate space for indices 0,1,2.  Index 0 is unused. */
        cache = (seq_t *)calloc(3, sizeof(seq_t));
        if (cache == NULL) abort();
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_cap = 3;
        computed  = 2;
    }

    /* Return a pre‑computed value if requested. */
    if (n <= computed)
        return cache[n];

    /* Ensure enough space for all indices up to n. */
    ensure_capacity(n);

    /* Compute all missing terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* a(i-1) is already known. */
        seq_t prev       = cache[i - 1];          /* a(i-1)          */
        seq_t two_prev   = prev + prev;           /* 2*a(i-1)        */
        seq_t abs_term;

        /* abs(2*a(i-1) - i) */
        if (two_prev < (seq_t)i)
            abs_term = (seq_t)i - two_prev;
        else
            abs_term = two_prev - (seq_t)i;

        /* a(a(a(i-1))) : the indices used are guaranteed < i
           because, for k≥3, a(k)<k.  We cast the intermediate seq_t
           values to idx_t for indexing. */
        seq_t a1 = cache[(idx_t)prev];            /* a(a(i-1))  */
        seq_t a2 = cache[(idx_t)a1];              /* a(a(a(i-1))) */

        cache[i] = a2 + abs_term;
    }

    computed = n;
    return cache[n];
}
