/*
 * A165266  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A165266 : Number of reduced words of length n in
 *  the Coxeter group with 12 generators S_i and relations
 *  (S_i)^2 = (S_i S_j)^9 = I.
 *
 *  Recurrence (from the generating function):
 *      a(0) = 1
 *      a(n) = 10·a(n‑1)+⋯+10·a(n‑8) – 55·a(n‑9) + p_n   (n ≥ 0)
 *  where
 *      p_0 = 1,  p_1 = … = p_8 = 2,  p_9 = 1,  p_k = 0 (k > 9).
 *
 *  The implementation uses a static cache so that calls in any order are
 *  answered efficiently (O(n) total time).  The cache is grown with
 *  malloc/realloc when compiled as C, and with new/delete when compiled
 *  as C++ (this ensures proper construction when seq_t is a class type).
 */

#include <stdlib.h>              /* for malloc, realloc, abort */
#ifdef __cplusplus
#  include <cstddef>            /* for size_t */
#  include <new>                /* for operator new/delete */
#endif

/* Pascal‑triangle of the numerator of the generating function. */
static const seq_t pcoeff[10] = {
    (seq_t)1ull, (seq_t)2ull, (seq_t)2ull, (seq_t)2ull,
    (seq_t)2ull, (seq_t)2ull, (seq_t)2ull, (seq_t)2ull,
    (seq_t)2ull, (seq_t)1ull
};

static seq_t *cache   = NULL;   /* dynamic array of computed terms   */
static idx_t   size   = 0;       /* current number of stored terms    */

static void grow_cache(idx_t new_size)
{
#ifdef __cplusplus
    /* Allocate new array with C++ operator new.  The old array is
     * deleted after copying its elements, guaranteeing proper
     * construction/destruction for class types. */
    seq_t *new_cache = new seq_t[new_size];
    for (idx_t i = 0; i < size; ++i) new_cache[i] = cache[i];
    delete[] cache;               /* works for both built‑in and class types */
    cache = new_cache;
#else
    /* For C, use realloc; cache elements are trivially copyable. */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
#endif
    size = new_size;
}

seq_t a(idx_t n)
{
    /* Ensure the cache can hold up to index n. */
    if (n + 1U > size) {
        /* Remember the old size to compute the missing terms. */
        idx_t old_size = size;
        grow_cache(n + 1U);
        /* Compute new terms from the old size up to n. */
        for (idx_t k = old_size; k <= n; ++k) {
            seq_t term = 0;

            /* Sum 10 * a(k-i) for i = 1..8, if k-i exists. */
            for (int i = 1; i <= 8; ++i) {
                if (k >= (idx_t)i) {
                    term += (seq_t)10 * cache[k - i];
                }
            }
            /* Subtract 55 * a(k-9) if k >= 9. */
            if (k >= 9U) {
                term -= (seq_t)55 * cache[k - 9U];
            }
            /* Add the constant term p_k (zero for k > 9). */
            if (k <= 9U) {
                term += pcoeff[(int)k];
            }
            cache[k] = term;
        }
        /* size already set to n+1U inside grow_cache. */
    } else {
        /* cache already large enough; no new terms needed. */
    }

    return cache[n];
}
