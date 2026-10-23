/*
 * A169085  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A169085: a(0)=1; a(1)=24; a(n)=24·23^(n-1) for 1≤n≤24;
   for n≥25: a(n)=22·∑_{k=1}^{24} a(n−k) – 253·a(n−25). */

/* Standard header for malloc/free */
#include <stdlib.h>

/* Forward declaration of auxiliary alloc/dealloc when compiling as C++ */
#ifdef __cplusplus
# include <new>
#endif

/* Static cache to memoize previously computed terms */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* Total allocated capacity */
static idx_t cache_size = 0;  /* Number of terms currently stored (== cache_size) */

/* Allocate a new buffer and copy existing terms */
static void grow_cache_to(idx_t n)
{
    /* Grow only if necessary */
    if (n < cache_size) return;

    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= n) new_cap <<= 1;

    /* Allocate new buffer */
    seq_t *new_buf = NULL;
#ifdef __cplusplus
    new_buf = new seq_t[new_cap];
#else
    new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_buf) exit(1);  /* Allocation failed */

    /* Copy existing values */
    for (idx_t i = 0; i < cache_size; ++i)
        new_buf[i] = cache[i];

    /* Deallocate old buffer */
#ifdef __cplusplus
    delete [] cache;
#else
    free(cache);
#endif

    cache = new_buf;
    cache_cap = new_cap;
    cache_size = n + 1;    /* store terms up to index n inclusive */
}

/* Compute a(n) with memoization */
seq_t a(idx_t n)
{
    grow_cache_to(n);

    /* Compute new terms if needed */
    for (idx_t i = cache_size - 1; i <= n; ++i)  /* cache_size is n+1 after grow */
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;
        }
        else if (i == 1)
        {
            cache[1] = (seq_t)24;
        }
        else if (i <= 24)
        {
            cache[i] = cache[i - 1] * (seq_t)23;
        }
        else
        {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 24; ++k)
                sum += cache[i - k];
            cache[i] = sum * (seq_t)22 - cache[i - 25] * (seq_t)253;
        }
    }

    return cache[n];
}
