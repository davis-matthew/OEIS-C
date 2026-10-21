/*
 * A053496  Number of degree-n permutations of order dividing 6.
 * https://oeis.org/A053496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=18, a(5)=66 a(n) = a(n-1) +
 *            (n-1)*a(n-2) + (n-1)*(n-2)*a(n-3) +
 *            (n-5)*(n-1)*(n-2)*(n-3)*(n-4)*a(n-6) for n>=6
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are provided by the build.  */

#include <stdlib.h>

/* Cache for previously computed values */
static seq_t *cache = NULL;
static size_t  cache_cap = 0;
/* Highest index that has been computed; -1 means none yet. */
static idx_t  cache_upto = (idx_t)-1;

/* Ensure cache large enough to hold indices up to n */
static void ensure_cache_cap(idx_t n)
{
    size_t need = (size_t)(n + 1);
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need)
        new_cap <<= 1;
#ifdef __cplusplus
    seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialize first six terms */
static void init_base_terms(void)
{
    /* Guarantee cache holds indices 0..5 */
    ensure_cache_cap(5);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)6;
    cache[4] = (seq_t)18;
    cache[5] = (seq_t)66;
    cache_upto = 5;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    /* Allocate cache if necessary */
    ensure_cache_cap(n);

    /* Initialise base values once */
    if (cache_upto == (idx_t)-1)
        init_base_terms();

    /* Compute missing values up to n */
    for (idx_t i = cache_upto + 1; i <= n; ++i)
    {
        if (i <= 5)  /* already known */
            continue;

        /* a(i-1) */
        seq_t term1 = cache[(size_t)(i - 1)];

        /* (i-1)*a(i-2) */
        seq_t term2 = (seq_t)(i - 1) * cache[(size_t)(i - 2)];

        /* (i-1)*(i-2)*a(i-3) */
        seq_t term3 = (seq_t)(i - 1) * (seq_t)(i - 2) * cache[(size_t)(i - 3)];

        /* (i-5)*(i-1)*(i-2)*(i-3)*(i-4)*a(i-6) */
        seq_t term4 =
            (seq_t)(i - 5) * (seq_t)(i - 1) * (seq_t)(i - 2) *
            (seq_t)(i - 3) * (seq_t)(i - 4) * cache[(size_t)(i - 6)];

        cache[(size_t)i] = term1 + term2 + term3 + term4;
    }

    cache_upto = n;
    return cache[(size_t)n];
}
