/*
 * A039535  Numbers whose base-12 representation has the same nonzero number
 *          of 4's and 9's.
 * https://oeis.org/A039535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A039535:
   a(n) = the n-th integer whose base‑12 representation contains
   the same non‑zero number of digits 4 and 9. */

#include <stdlib.h>
#include <stdbool.h>

/* Check whether a number satisfies the property.
   Count the digits 4 and 9 in base‑12. */
static bool base12_ok(seq_t val)
{
    unsigned int cnt4 = 0, cnt9 = 0;
    while (val > 0) {
        unsigned int digit = (unsigned int)(val % 12);
        if (digit == 4) ++cnt4;
        else if (digit == 9) ++cnt9;
        val /= 12;
    }
    return cnt4 > 0 && cnt4 == cnt9;
}

/* Ensure the cache has at least 'needed' capacity. */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed <= *cap) return;
    size_t newcap = (*cap == 0) ? 16 : *cap * 2;
    while (newcap < needed) newcap *= 2;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

seq_t a(idx_t n)
{
    /* Static cache of qualifying numbers. */
    static seq_t *cache = NULL;
    static idx_t cache_cnt = 0;      /* number of items stored   */
    static size_t cache_cap = 0;     /* allocated capacity       */
    static idx_t next_candidate = 1; /* next integer to test     */

    /* Extend cache until we have at least n elements. */
    while (cache_cnt < n) {
        seq_t val = (seq_t)next_candidate++;
        if (base12_ok(val)) {
            ensure_capacity(cache_cnt + 1, &cache_cap, &cache);
            cache[cache_cnt++] = val;
        }
    }
    return cache[n - 1];
}
