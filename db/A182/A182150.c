/*
 * A182150  Semiprimes that are also semiprime when their digits are sorted
 *          into nondecreasing order.
 * https://oeis.org/A182150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 879 (2026-11-03)
 * Helpers:   bigomega_seq, digit_frequencies, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A182150: semiprimes that are also semiprime when their digits
 * are sorted into nondecreasing order. This implementation
 * incrementally searches integers and caches the found terms so that
 * any order of calls returns the correct n‑th value. */

#include <stddef.h>                     /* size_t */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/bigomega_seq.h"
#include "../utils/digit_frequencies.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size) /* elem_size: sizeof(T) */
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, new_cap);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    /* Static cache: cache[0] = a(1). */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* The next integer to test (last checked candidate). */
    static seq_t last_checked = 3;  /* initialised to 3, so first test is 4 */

    /* Ensure the cache holds at least n terms. */
    while (cache_size < n)
    {
        /* Allocate or expand the cache. */
        if (!cache)
        {
            cache_cap = 64;
            cache = alloc_seq_array(cache_cap);
        }
        else if (cache_size >= cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            ensure_capacity(new_cap, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        }

        /* Next candidate integer. */
        ++last_checked;
        const seq_t cand = last_checked;

        /* Test whether cand is a semiprime. */
        if (bigomega_seq((uint64_t)cand) == 2)
        {
            /* Compute the integer formed by sorting cand's digits */
            unsigned int freq[10] = {0};
            digit_frequencies(cand, freq);

            seq_t sorted = 0;
            for (int d = 0; d < 10; ++d)
            {
                for (unsigned int k = 0; k < freq[d]; ++k)
                {
                    sorted = sorted * 10 + d;
                }
            }

            /* Check that the sorted integer is also a semiprime. */
            if (bigomega_seq((uint64_t)sorted) == 2)
            {
                cache[cache_size++] = cand;
            }
        }
    }

    return cache[n - 1];
}
