/*
 * A221567  Number of 0..2 arrays of length n with each element differing from
 *          at least one neighbor by something other than 1.
 * https://oeis.org/A221567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 4*a(n-4), with a(1)=0, a(2)=5,
 *            a(3)=9, a(4)=25. This recurrence was found empirically (see OEIS
 *            A221567). The code uses memoization to support arbitrary call
 *            order
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
/* In C++ use new/delete for allocation of seq_t.  The build supplies
   seq_t as a (possibly arbitrary‑precision) integer type, so we
   default‑construct each element, which is safe. */
#endif

static seq_t *cache = NULL;        /* array holding a(1)...a(max_computed) */
static size_t cache_size = 0;      /* allocated capacity */
static size_t max_computed = 0;    /* highest n for which a(n) is stored */

/* Expand the cache to at least 'needed' elements and compute any
   missing values up to 'needed'.  The cache stores a(1) at index 0. */
static void ensure_computed(idx_t needed)
{
    if (needed <= max_computed) return;

    /* Allocate or enlarge the cache. */
    if (needed > cache_size) {
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[needed];
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            delete[] cache;
        }
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
        if (!new_cache) abort();      /* allocation failure */
        cache = new_cache;
#endif
        cache_size = needed;
    }

    /* Compute missing entries from max_computed+1 up to needed. */
    for (idx_t n = (idx_t)max_computed + 1; n <= needed; ++n) {
        if (n == 1) {
            cache[0] = (seq_t)0;
        } else if (n == 2) {
            cache[1] = (seq_t)5;
        } else if (n == 3) {
            cache[2] = (seq_t)9;
        } else if (n == 4) {
            cache[3] = (seq_t)25;
        } else {
            /* a(n) = 3*a(n-1) - 2*a(n-2) + 4*a(n-4) */
            seq_t term1 = 3 * cache[n - 2];          /* a(n-1) */
            seq_t term2 = 2 * cache[n - 3];          /* a(n-2) */
            seq_t term4 = 4 * cache[n - 5];          /* a(n-4) */
            cache[n - 1] = term1 - term2 + term4;
        }
    }
    max_computed = needed;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n<1; return 0 */
    ensure_computed(n);
    return cache[n - 1];
}
