/*
 * A128022  a(1)=1, a(n) = the multiple of n which is >= (sum{k=1 to n-1}
 *          a(k)) and is < (n + sum{k=1 to n-1} a(k)).
 * https://oeis.org/A128022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * ceil( (sum_{k=1}^{n-1} a(k)) / n ) a(1) = 1 (Indices
 *            start at 1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t   *cache      = NULL;   /* memoised values a(1)...a(m)                 */
static size_t  cache_cap   = 0;      /* allocated size of cache                   */
static idx_t   cache_len   = 0;      /* highest n already computed                */
static seq_t   cache_sum   = 0;      /* sum of a(1)...a(cache_len)                */

/* Ensure the cache can hold at least n elements. */
static void ensure_capacity(idx_t n)
{
    if (n <= cache_cap)
        return;

#ifdef __cplusplus
    seq_t *new_ptr = new seq_t[n];
#else
    seq_t *new_ptr = (seq_t*)malloc(n * sizeof(seq_t));
#endif
    if (!new_ptr)
        exit(1);                     /* allocation failed */

    /* Copy existing elements via assignment to respect any non‑trivial copy */
    for (size_t i = 0; i < cache_len; ++i)
        new_ptr[i] = cache[i];

#ifdef __cplusplus
    delete [] cache;
#else
    free(cache);
#endif
    cache = new_ptr;
    cache_cap = (size_t)n;
}

/* Compute the nth term of OEIS A128022. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined for n=0, return 0            */

    if (n <= cache_len)
        return cache[n - 1];

    /* Initialise the first term if necessary. */
    if (cache_len == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
        cache_sum = (seq_t)1;
    }

    if (n > cache_cap)
        ensure_capacity(n);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* ceil division: ceil(cache_sum / i) = (cache_sum + i - 1) / i */
        seq_t ceildiv = (cache_sum + (seq_t)i - (seq_t)1) / (seq_t)i;
        seq_t ai      = (seq_t)i * ceildiv;
        cache[i - 1]  = ai;
        cache_sum    += ai;
    }

    cache_len = n;
    return cache[n - 1];
}
