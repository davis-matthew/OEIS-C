/*
 * A133409  Zero followed by partial sums of A133405.
 * https://oeis.org/A133409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=a(2)=a(3)=0, a(4)=1
 *            a(n)=4*a(n-1)-3*a(n-2)-a(n-3)+4*a(n-4)-3*a(n-5) for n≥5 (This
 *            recurrence follows from the OEIS relations.)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of previously computed terms.  In C++ the cast ensures the allocation
   produces a pointer to the correct type (the big‑integer class is
   construction‑invariant for default values). */
static seq_t *cache = NULL;      /* pointer to cache array */
static idx_t cap   = 0;          /* total allocated capacity          */
static idx_t size  = 0;          /* number of initialized elements    */

/* Initialise the cache with the first known terms (a0..a4). */
static void init_cache(void)
{
    if (cache != NULL)
        return; /* already initialised */

    /* Allocate a small initial buffer.  The cast is necessary for C++ */
    cap   = 16;
    cache = (seq_t *)calloc(cap, sizeof(seq_t));
    if (!cache)
        exit(1);

    /* seed the first five terms */
    cache[0] = 0;
    cache[1] = 0;
    cache[2] = 0;
    cache[3] = 0;
    cache[4] = 1;
    size = 5; /* number of valid entries */
}

/* Grow the cache so that it contains at least indices 0..n. */
static void ensure_capacity(idx_t n)
{
    if (n < size)
        return;

    /* double capacity until it is large enough */
    idx_t new_cap = cap;
    while (new_cap <= n)
        new_cap <<= 1;

    /* reallocate; cast for C++ */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);

    cache = new_cache;
    cap   = new_cap;

    /* compute any new terms that were not already present */
    for (idx_t i = size; i <= n; ++i) {
        if (i <= 4) /* already set in init_cache */ continue;
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        seq_t t5 = cache[i - 5];
        cache[i] = 4 * t1 - 3 * t2 - t3 + 4 * t4 - 3 * t5;
    }
    size = n + 1;
}

/*
 * Public function: compute a(n) for any n >= 0.
 * The sequence is memoised; callers may request indices in arbitrary order.
 */
seq_t a(idx_t n)
{
    init_cache();
    if (n >= size)
        ensure_capacity(n);
    return cache[n];
}
