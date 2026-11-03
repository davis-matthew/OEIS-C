/*
 * A068780  Composite numbers n such that n+1 is also composite.
 * https://oeis.org/A068780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9078 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068780: composite numbers n such that n+1 is also composite */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
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

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_size = 0;        /* number of cached terms */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static idx_t last_checked = 1;       /* last integer examined for the pair */

static void grow_cache(idx_t target) {
    while (cache_size < (size_t)target) {
        last_checked++;   /* move to next integer */
        /* both n and n+1 must be composite */
        if (!is_prime_seq((seq_t)last_checked) &&
            !is_prime_seq((seq_t)(last_checked + 1))) {
            /* ensure we have space for a new entry */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                ensure_capacity(new_cap, &cache_cap, OEISC_VPP(&cache),
                                sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)last_checked;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for n=0, return 0 */
    /* grow cache until it contains at least n terms */
    if (cache_size < (size_t)n) grow_cache(n);
    return cache[n - 1];
}
