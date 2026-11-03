/*
 * A062909  Non-palindromic number and its reversal are both multiples of 4.
 * https://oeis.org/A062909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   This function generates the nth non‑palindromic multiple of 4 *
 *            for which its decimal reversal is also a multiple of 4. The
 *            sequence is * searched incrementally by testing successive
 *            multiples of 4
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 975 (2026-11-03)
 * Helpers:   is_palindrome_base, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/reverse_digits.h"
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
    if (n == 0)                      /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    /* Static cache of already generated terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;    /* number of terms stored   */
    static size_t cache_cap  = 0;    /* allocated capacity       */
    static seq_t last_checked = 0;   /* largest number examined   */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t cand = last_checked + 4;     /* next candidate (multiple of 4) */

        /* Scan forward for the next valid term. */
        for (;;) {
            /* Safety: insure the first candidate is at least 4. */
            if (cand < 4) cand = 4;

            /* Reject palindromic candidates. */
            if (is_palindrome_base(cand, 10)) {
                cand += 4;
                continue;
            }

            /* The reversal must also be a multiple of 4. */
            seq_t rev = reverse_digits(cand);
            if ((rev % 4) == 0) {
                /* Append the valid term to the cache. */
                ensure_capacity(cache_len + 1, &cache_cap,
                                OEISC_VPP(&cache), sizeof(seq_t));
                cache[cache_len++] = cand;
                last_checked = cand;   /* next search starts after this value */
                break;                 /* outer while loop will check if more needed */
            }

            cand += 4;                 /* try the next multiple of 4 */
        }
    }

    return cache[n - 1];
}
