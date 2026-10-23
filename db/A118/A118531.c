/*
 * A118531  Start with 1 and repeatedly reverse the digits and add 14 to get
 *          the next term.
 * https://oeis.org/A118531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=reverse(a(n-1))+14, for n>1 */

#include <stdlib.h>
#include "../utils/reverse_decimal.h"

/* static cache of computed terms */
static seq_t *cache     = NULL;  /* cache[0] == a(1) */
static size_t cache_cap = 0;     /* allocated capacity in terms */
static idx_t  cache_len = 0;     /* number of terms actually stored */

/* Ensure the cache can hold at least n terms */
static void ensure_capacity(idx_t n)
{
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* In an actual program you should check for allocation failure.
           For brevity we assume success here. */
        cache     = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* behaviour below the offset is irrelevant */

    /* initialise cache with first term if empty */
    if (cache_len == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* extend cache up to requested index */
    if (n > cache_len) {
        ensure_capacity(n);
        for (idx_t i = cache_len; i < n; ++i) {
            seq_t val = cache[i - 1];
            val = reverse_decimal(val) + (seq_t)14;
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
