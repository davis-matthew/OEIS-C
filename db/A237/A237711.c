/*
 * A237711  The number of P-positions in the game of Nim with up to four
 *          piles, allowing for piles of zero, such that the total number of
 *          objects in all piles is 2n.
 * https://oeis.org/A237711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(2n+1) = 6 * a(n) a(2n+2) = a(n+1) + a(n) (for n ≥ 0)
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Uses a lazy memoisation cache that grows only as needed.
    The implementation works both in C11 and C++17.  */

#ifdef __cplusplus
#include <new>
#endif

#include <stdlib.h>   /* malloc, free, abort */
#include <string.h>   /* memcpy, memset */

/* static cache storage */
static seq_t *cache_ptr    = NULL;             /* pointer to cached values   */
static size_t cache_cap    = 0;                /* allocated cache capacity   */
static size_t computed_max = (size_t)-1;       /* largest index computed yet */

/* Ensure that cache_ptr has room for indices 0 .. needed-1  */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;      /* already large enough */

    size_t new_cap = needed;
    seq_t *new_cache;

#ifdef __cplusplus
    /* allocate uninitialized memory, then copy old values */
    new_cache = reinterpret_cast<seq_t*>(::operator new[](new_cap * sizeof(seq_t)));
    if (cache_ptr) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache_ptr[i];
        /* the remaining elements are uninitialised – they will be
           overwritten when we compute them. */
        ::operator delete[](cache_ptr);
    }
#else
    new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    if (cache_ptr) {
        memcpy(new_cache, cache_ptr, cache_cap * sizeof(seq_t));
        free(cache_ptr);
    }
#endif

    cache_ptr = new_cache;
    cache_cap = new_cap;
}

/* the required OEIS sequence function */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (computed_max == (size_t)-1) {
        ensure_cache_cap(1);          /* at least room for index 0   */
        cache_ptr[0] = (seq_t)1;      /* a(0) = 1                    */
        computed_max = 0;
    }

    size_t idx = (size_t)n;
    if (idx > computed_max) {                 /* compute up to idx          */
        ensure_cache_cap(idx + 1);             /* capacity for idx           */
        for (size_t i = computed_max + 1; i <= idx; ++i) {
            if ((i & 1U) != 0U) {             /* odd: a(2n+1) = 6*a(n)      */
                size_t k = (i - 1U) >> 1U;
                cache_ptr[i] = cache_ptr[k] * (seq_t)6;
            } else {                          /* even: a(2n+2) = a(n+1)+a(n)*/
                size_t q = i >> 1U;           /* q = n+1                     */
                cache_ptr[i] = cache_ptr[q] + cache_ptr[q - 1U];
            }
        }
        computed_max = idx;
    }

    return cache_ptr[idx];
}
