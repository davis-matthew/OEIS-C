/*
 * A227410  Palindromic prime numbers representing a date in "condensed
 *          American notation" MMDDYY.
 * https://oeis.org/A227410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, less
 * Formula:   a(n) is the n‑th palindromic prime that can be read as a date in
 *            the condensed American notation MMDDYY. The dates are restricted
 *            to non‑leap years (February always 28 days). The function
 *            generates all such dates, filters palindromic primes, sorts
 *            them, and returns the (n‑1)th element of this lis...
 * Range:     u8 none, u16 n <= 8, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: full, pass, 27 terms through n = 27 (2026-11-03)
 * Helpers:   is_palindrome_base, is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
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

seq_t a(idx_t n) {
    /* Cache of all palindromic prime dates */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;

    if (sz == 0) {
        /* Build list on first call */
        for (idx_t yy = 0; yy < 100; ++yy) {          /* year 00–99   */
            for (idx_t mm = 1; mm <= 12; ++mm) {      /* month 01–12 */
                idx_t days;
                switch (mm) {
                    case 2: days = 28; break;          /* February non‑leap */
                    case 4: case 6: case 9: case 11: days = 30; break;
                    default: days = 31; break;
                }
                for (idx_t dd = 1; dd <= days; ++dd) { /* day 01…days */
                    seq_t val = (seq_t)mm * 10000
                              + (seq_t)dd * 100
                              + (seq_t)yy;              /* MMDDYY */
                    if (is_palindrome_base(val, 10) &&
                        is_prime_seq((idx_t)val)) {
                        if (sz == cap) {
                            size_t ncap = cap == 0 ? 8 : cap * 2;
                            ensure_capacity(ncap, &cap, OEISC_VPP(&cache),
                                            sizeof(seq_t));
                        }
                        cache[sz++] = val;
                    }
                }
            }
        }
        /* Sort in ascending order */
        mem_qsort(cache, sz, sizeof(seq_t), (int(*)(const void*, const void*))cmp_seq_t);
    }

    if (n == 0 || n > sz) return 0;   /* undefined beyond the list size */
    return cache[n - 1];
}
