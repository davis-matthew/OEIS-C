/*
 * A126606  Fixed point of transformation of the seed sequence {0,2}.
 * https://oeis.org/A126606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * A002487(n-1). Recurrence: a(1)=0, a(2)=2, a(2n) =
 *            a(n) + a(n+1), a(2n+1) = a(n+1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised and works for any order of calls. */

#include <stdlib.h>   /* malloc, realloc, calloc */

static seq_t *cache = NULL;          /* cache[1..] holds a(n) */
static idx_t  cache_cap = 0;           /* allocated size (>=0) */
static idx_t  computed_up_to = 0;      /* highest index computed */

/* Ensure cache has room for indices up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if (cache_cap >= n + 1)
        return;                     /* already sufficient */

    size_t new_cap = (cache_cap == 0) ? 3 : (size_t)cache_cap * 2;
    while (new_cap <= n)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL)
        abort();                    /* out of memory */

    /* Initialise newly allocated slots to zero. */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;
    cache_cap = (idx_t)new_cap;
}

/* Initialise cache with base cases a(1)=0, a(2)=2. */
static void
init_cache(void)
{
    if (cache != NULL)
        return;

    /* allocate indices 0..2 (index 0 unused) */
    cache_cap = 3;
    cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    if (cache == NULL)
        abort();

    cache[1] = (seq_t)0;   /* a(1) */
    cache[2] = (seq_t)2;   /* a(2) */
    computed_up_to = 2;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1 */

    init_cache();
    ensure_cache(n);

    /* If value is already known, return it. */
    if (n <= computed_up_to)
        return cache[n];

    /* Compute missing values up to n. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if ((i & 1) == 0) {            /* i is even: i = 2k */
            idx_t k = i >> 1;
            cache[i] = cache[k] + cache[k + 1];
        } else {                       /* i is odd: i = 2k+1 */
            idx_t k = (i - 1) >> 1;
            cache[i] = cache[k + 1];
        }
    }
    computed_up_to = n;
    return cache[n];
}
