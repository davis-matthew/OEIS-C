/*
 * A221455  Number of 0..4 arrays of length n with each element unequal to at
 *          least one neighbor, with new values introduced in 0..4 order.
 * https://oeis.org/A221455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A221455 – Number of 0..4 arrays of length n with each element
 * unequal to at least one neighbour, new values introduced in 0..4 order.
 *
 * Recurrence (for n ≥ 7):
 *   a(n) = 7*a(n-1) - 7*a(n-2) - 20*a(n-3) + 10*a(n-4) + 24*a(n-5) + 8*a(n-6)
 * Initial values:
 *   a(1)=0, a(2)=1, a(3)=2, a(4)=7, a(5)=25, a(6)=101
 *
 * The implementation keeps a dynamic cache that stores all computed terms.
 * It allocates memory with malloc on a C build and with new[] on a C++ build,
 * ensuring all constructors are invoked when seq_t has a non‑trivial
 * constructor.  The cache is grown only when a larger index is requested.
 */

#include <stdlib.h>  /* malloc, free */

static seq_t *seq_cache = NULL;   /* cache for terms, index 0 unused */
static idx_t   cache_size = 0;    /* number of elements allocated (index 0..cache_size-1) */
static idx_t   computed  = 0;     /* highest index whose value is known */

/* Allocate or grow the cache to accommodate indices up to 'n' (inclusive) */
static void prepare_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;                    /* already large enough */

    idx_t new_size = n + 1;         /* include unused 0 slot */
    seq_t *new_cache;

#ifdef __cplusplus
    new_cache = new seq_t[new_size];
#else
    new_cache = malloc(new_size * sizeof(seq_t));
#endif
    if (!new_cache)
        return;                    /* allocation failed – keep old cache */

    /* copy old data */
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = seq_cache[i];

#ifdef __cplusplus
    delete[] seq_cache;
#else
    free(seq_cache);
#endif
    seq_cache = new_cache;
    cache_size = new_size;
}

/* Ensure initial values are present in the cache */
static void init_base_values(void)
{
    prepare_cache(6);              /* need indices 0..6 */

    seq_cache[1] = (seq_t)0;
    seq_cache[2] = (seq_t)1;
    seq_cache[3] = (seq_t)2;
    seq_cache[4] = (seq_t)7;
    seq_cache[5] = (seq_t)25;
    seq_cache[6] = (seq_t)101;

    computed = 6;
}

/* Compute terms from 'start' up to 'end' inclusive, assuming start-1
   has already been computed (start ≥ 7).  The cache must be big enough. */
static void compute_terms(idx_t start, idx_t end)
{
    for (idx_t i = start; i <= end; ++i)
    {
        seq_t a1 = 7 * seq_cache[i - 1];
        seq_t a2 = 7 * seq_cache[i - 2];
        seq_t a3 = 20 * seq_cache[i - 3];
        seq_t a4 = 10 * seq_cache[i - 4];
        seq_t a5 = 24 * seq_cache[i - 5];
        seq_t a6 = 8 * seq_cache[i - 6];

        seq_cache[i] = a1 + a4 + a5 + a6 - (a2 + a3);
    }
    if (end > computed)
        computed = end;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* out of range – behaviour is undefined, but return 0 */
        return (seq_t)0;

    /* initialise base values on first call */
    if (computed == 0)
        init_base_values();

    /* grow cache if needed */
    if (n + 1 > cache_size)
        prepare_cache(n);

    /* compute any missing terms */
    if (n > computed)
        compute_terms(computed + 1, n);

    return seq_cache[n];
}
