/*
 * A221968  Number of -n..n arrays of length 5 with the sum ahead of each
 *          element differing from the sum following that element by n or
 *          less.
 * https://oeis.org/A221968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) * - 15*a(n-4) + 6*a(n-5)
 *            - a(n-6) (n > 6) * Initial values: a(1)=63, a(2)=705, a(3)=3647,
 *            * a(4)=12609, a(5)=34111, a(6)=78273
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 57, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 63 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include <stdint.h>          /* for uint64_t */
#include <stdbool.h>
#include <stdlib.h>
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

#undef WARN_UNUSED_RESULT
#define WARN_UNUSED_RESULT
seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed_up_to = 0; /* highest index we have computed */

    if (n < 1) return 0;           /* undefined for n < 1 */

    /* we need at least indices 0..max(n,6) in the cache */
    idx_t needed = (n + 1 > 7) ? (n + 1) : 7;
    size_t old_cap = cache_cap;
    ensure_capacity(needed, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
    /* zero new entries */
    for (size_t i = old_cap; i < cache_cap; ++i) cache[i] = 0;

    /* initialise first six terms once */
    if (computed_up_to == 0) {
        cache[1] = 63;
        cache[2] = 705;
        cache[3] = 3647;
        cache[4] = 12609;
        cache[5] = 34111;
        cache[6] = 78273;
        computed_up_to = 6;
    }

    /* compute missing terms up to n using recurrence */
    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            seq_t a6 = cache[i - 6];
            cache[i] = 6 * a1 - 15 * a2 + 20 * a3 - 15 * a4 + 6 * a5 - a6;
        }
        computed_up_to = n;
    }

    return cache[n];
}
