/*
 * A039564  Numbers whose base-5 representation has the same number of 0's,
 *          1's and 3's.
 * https://oeis.org/A039564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: base‑5 representation has equal counts of digits 0, 1 and 3 */
/* The function returns the n‑th such number (offset 1). */
#include <stdlib.h>
#include <stdbool.h>

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t   cache_size = 0;
    static size_t  cache_cap  = 0;
    static idx_t   next_candidate = 1;

    /* Return cached value if already computed. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Generate terms until we reach the desired n. */
    while (cache_size < n) {
        seq_t cand = (seq_t)next_candidate++;
        seq_t y = cand;
        unsigned int cnt0 = 0, cnt1 = 0, cnt3 = 0;

        while (y > (seq_t)0) {
            unsigned int d = (unsigned int)(y % (seq_t)5);
            if (d == 0)
                cnt0++;
            else if (d == 1)
                cnt1++;
            else if (d == 3)
                cnt3++;
            y = y / (seq_t)5;
        }

        if (cnt0 == cnt1 && cnt1 == cnt3) {
            if (cache_size == cache_cap)
                ensure_capacity(cache_size + 1, &cache_cap,
                                (void **)&cache, sizeof(seq_t));
            cache[cache_size++] = cand;
        }
    }

    return cache[n - 1];
}
