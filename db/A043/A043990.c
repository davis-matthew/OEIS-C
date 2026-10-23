/*
 * A043990  Numbers k such that 5 and 6 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has56or65
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A043990 – Numbers k such that 5 and 6 occur juxtaposed (in any
    order) in the base‑9 representation of k but not in k+1.  The function
    builds the sequence lazily and caches already found values.  It does
    not assume increasing calls.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has56or65.h"

/* Ensure a dynamic array is large enough. */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        if (new_cap < need)
            new_cap = need;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    if (n < (idx_t)1)
        return (seq_t)0;      /* undefined for n < 1 */

    size_t target = (size_t)n;
    if (cache_len < target) {
        /* Continue searching from the last found value + 1 */
        seq_t start = (cache_len ? cache[cache_len - 1] + (seq_t)1 : (seq_t)1);
        for (seq_t k = start; cache_len < target; ++k) {
            if (has56or65(k) && !has56or65(k + (seq_t)1)) {
                grow_seq(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = k;
            }
        }
    }
    return cache[n - (idx_t)1];
}
