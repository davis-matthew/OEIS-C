/*
 * A039256  Numbers whose base-12 representation has the same number of 5's
 *          and 8's.
 * https://oeis.org/A039256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑12 representation has the same number of
 * 5's and 8's (OEIS A039256).  The function computes the nth term
 * using a simple brute‑force scan, caching results for subsequent
 * calls.  The implementation is fully generic for seq_t and idx_t
 * and compiles as C11 and C++17. */

#include <stdint.h>
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

/* test whether a non‑negative seq_t has equal counts of digits 5 and 8
 * in its base‑12 representation */
static bool base12_has_equal_5_8(seq_t x)
{
    unsigned int cnt5 = 0, cnt8 = 0;
    if (x == 0) return true;            /* 0 has zero 5's and zero 8's */
    seq_t tmp = x;
    while (tmp > 0) {
        seq_t digit = tmp % 12;
        if (digit == 5) ++cnt5;
        else if (digit == 8) ++cnt8;
        tmp = tmp / 12;
    }
    return cnt5 == cnt8;
}

/* static cache */
static seq_t *cache = NULL;               /* array of found numbers */
static size_t cache_len = 0;               /* number of cached values (a(1)…a(cache_len)) */
static size_t cache_cap = 0;               /* capacity of the cache array */
static idx_t last_scanned = 0;             /* next integer to test after the last cached one */

seq_t a(idx_t n)
{
    /* Guard against n == 0, though the sequence is defined for n >= 1 */
    if (n == 0) return 0;

    /* Generate and cache values up to index n on demand */
    while (cache_len < (size_t)n) {
        /* Find the next integer that satisfies the base‑12 property */
        while (!base12_has_equal_5_8((seq_t)last_scanned))
            ++last_scanned;

        /* Ensure the cache array has space for another element */
        ensure_capacity(cache_len + 1, &cache_cap,
                        OEISC_VPP(&cache), sizeof(seq_t));

        /* Store the found number and advance state */
        cache[cache_len] = (seq_t)last_scanned;
        ++cache_len;
        ++last_scanned;
    }

    return cache[(size_t)n - 1];
}
