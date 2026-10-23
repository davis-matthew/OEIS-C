/*
 * A048773  Partial sums of A048697.
 * https://oeis.org/A048773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, changed
 * Formula:   a(n) = 2*a(n-1)+a(n-2)+9; a(0)=1, a(1)=11
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compile‑time type aliases are supplied by the build system. */
extern seq_t a(idx_t n);

/* --- Internal static cache -------------------------------------------------- */
static seq_t *seq_cache   = NULL;   /* array of computed values          */
static idx_t  cache_cap   = 0;       /* number of allocated elements      */
static idx_t  cache_max   = 0;       /* highest index that has been filled */
static int    cache_init  = 0;       /* cache has been initially filled?  */

/* Allocate or grow the cache to hold at least (need+1) elements. */
static void
grow_cache(idx_t need)
{
    if (cache_cap > need) return;                 /* already big enough */
    seq_t *new_cache = NULL;
#ifdef __cplusplus
    new_cache = new seq_t[need + 1];
#else
    new_cache = (seq_t *)malloc((need + 1) * sizeof(seq_t));
#endif
    /* copy old data if any */
    if (seq_cache != NULL) {
        for (idx_t i = 0; i < cache_cap; ++i) {
            new_cache[i] = seq_cache[i];
        }
#ifdef __cplusplus
        delete[] seq_cache;
#else
        free(seq_cache);
#endif
    }
    seq_cache = new_cache;
    cache_cap = need + 1;
}

/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* base cases are handled directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    /* initialise cache on first call */
    if (!cache_init) {
        grow_cache(n);                     /* enough space for the call */
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)11;
        cache_max   = 1;
        cache_init  = 1;
    }

    /* ensure we can store up to n */
    if (n + 1 > cache_cap) {
        grow_cache(n);
    }

    /* fill values from cache_max+1 up to n */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_cache[i] = seq_cache[i - 1] + seq_cache[i - 1] + seq_cache[i - 2] + (seq_t)9;
        }
        cache_max = n;
    }

    return seq_cache[n];
}
