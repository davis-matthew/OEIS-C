/*
 * A039149  Numbers whose base-10 representation has the same number of 5's
 *          and 9's.
 * https://oeis.org/A039149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   digit_frequencies, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches numbers in order and records those with equal counts of 5's and 9's. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_frequencies.h"
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

seq_t a(idx_t n)
{
    /* n is 1‑based; a(1)=0.  For n==0 we return 0 to avoid undefined behaviour. */
    if (n == 0) return 0;

    static seq_t *cache = NULL;     /* cached terms */
    static size_t cache_len = 0;    /* number of cached terms */
    static size_t cache_cap = 0;    /* allocated capacity */
    static seq_t current = 0;       /* last number examined (0‑based) */

    /* Ensure we have the first term (0) cached. */
    if (cache_len == 0) {
        ensure_capacity(1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = 0;
        cache_len = 1;
        current = 0;
    }

    /* If the requested index is already cached, return it. */
    if (cache_len >= (size_t)n) return cache[n - 1];

    /* Generate terms until we reach the required index. */
    while (cache_len < (size_t)n) {
        ++current;                     /* next integer to test */
        unsigned int freq[10] = {0};   /* digit counts */

        digit_frequencies(current, freq);
        if (freq[5] == freq[9]) {      /* equal number of 5's and 9's */
            ensure_capacity(cache_len + 1, &cache_cap,
                            OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_len++] = current;
        }
    }

    return cache[n - 1];
}
