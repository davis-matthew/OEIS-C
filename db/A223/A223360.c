/*
 * A223360  Rolling cube footprints: number of 5Xn 0..5 arrays starting with 0
 *          where 0..5 label faces of a cube and every array movement to a
 *          horizontal or antidiagonal neighbor moves across a corresponding
 *          cube edge.
 * https://oeis.org/A223360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence for n > 11: a(n) = 512*a(n-1) - 65536*a(n-2) +
 *            2490368*a(n-3) + 17825792*a(n-4) - 2818572288*a(n-5) +
 *            51539607552*a(n-6) - 274877906944*a(n-7) Initial values (n = 1 …
 *            11): a(1) = 1296 a(2) = 262144 a(3) = 84934656 a(4) =
 *            27518828544 a(5) = 9273505480704 a(6) = 3147420753985536 a(7) =
 *            1...
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 201 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

seq_t a(idx_t n) {
    if (n < 1) return 0; /* undefined, but return 0 */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* Ensure cache capacity up to n */
    if (cache == NULL || cache_len < n) {
        ensure_capacity((size_t)n, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        if (cache_len == 0) {
            /* Initialize cache with zeros if newly allocated */
            for (size_t i = 0; i < cache_cap; ++i) cache[i] = (seq_t)0;
        }
    }

    /* Base values (first 11 terms) */
    static seq_t base[11];
    static int base_init = 0;
    if (!base_init) {
        const char *base_str[] = {
            "1296",
            "262144",
            "84934656",
            "27518828544",
            "9273505480704",
            "3147420753985536",
            "1073434373061083136",
            "366633735578651197440",
            "125301697729353543057408",
            "42831316037653820665233408",
            "14641648774856138730026041344"
        };
        for (int i = 0; i < 11; ++i) base[i] = seq_from_str(base_str[i]);
        base_init = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 11) {
            val = base[i - 1];
        } else {
            /* Apply recurrence (indices in cache are zero-indexed) */
            val  = (512   * cache[i - 2])
                 - (65536 * cache[i - 3])
                 + (2490368 * cache[i - 4])
                 + (17825792 * cache[i - 5])
                 - (2818572288 * cache[i - 6])
                 + (51539607552 * cache[i - 7])
                 - (274877906944 * cache[i - 8]);
        }
        cache[i - 1] = val;
    }
    cache_len = n;
    return cache[n - 1];
}
