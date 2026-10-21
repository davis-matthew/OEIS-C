/*
 * A063338  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          65 ).
 * https://oeis.org/A063338
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13 Initial values
 *            (offset 2): a(2)=-1, a(3)=117, a(4)=272, a(5)=424, a(6)=576,
 *            a(7)=728, a(8)=880, a(9)=1032, a(10)=1184, a(11)=1336,
 *            a(12)=1488, a(13)=1644
 * Range:     i8 n <= 3, i16 n <= 42, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper: initialise the cache with known first values. */
static void init_cache(void);

/* Extend the cache to contain values up to at least n. */
static void ensure_cache(idx_t n);

/* Static storage for the table of values. */
static seq_t *cache = NULL;      /* pointer to array of values */
static idx_t  cache_size = 0;    /* allocated size of cache (in elements) */
static idx_t  max_computed = 13; /* highest index whose value is known */
static int    initialized = 0;   /* have we set up the base values? */

static void init_cache(void)
{
    if (initialized)
        return;
    /* Allocate space sufficient for indices 0..13. */
    cache_size = 14;
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    if (!cache)
        exit(1); /* allocation failed */

    /* Fill base values */
    cache[0] = 0;
    cache[1] = 0;
    cache[2] = (seq_t)(-1);
    cache[3] = (seq_t)117;
    cache[4] = (seq_t)272;
    cache[5] = (seq_t)424;
    cache[6] = (seq_t)576;
    cache[7] = (seq_t)728;
    cache[8] = (seq_t)880;
    cache[9] = (seq_t)1032;
    cache[10] = (seq_t)1184;
    cache[11] = (seq_t)1336;
    cache[12] = (seq_t)1488;
    cache[13] = (seq_t)1644;
    initialized = 1;
}

static void ensure_cache(idx_t n)
{
    if (n <= max_computed)
        return;                      /* already computed */
    if (n + 1 > cache_size) {        /* grow array if necessary */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            exit(1);
        cache = tmp;
        cache_size = (idx_t)new_size;
    }
    /* Compute new values using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i)
        cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
    max_computed = n;
}

/* Public function exposed to the rest of the program */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;               /* undefined for indices below the offset */
    init_cache();
    if (n <= max_computed)
        return cache[n];
    ensure_cache(n);
    return cache[n];
}
