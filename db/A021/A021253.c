/*
 * A021253  Decimal expansion of 1/249.
 * https://oeis.org/A021253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor((10^(n+1))/249) mod 10, i.e. the n‑th decimal digit
 *            (0‑based) of 1/249
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *digit_cache = NULL;   /* cached digits */
static idx_t cache_len   = 0;        /* number of digits cached */
static idx_t cache_cap   = 0;        /* allocation capacity */
static seq_t rem_val     = 1;        /* remainder before computing next digit */

seq_t a(idx_t n)
{
    /* Extend the cache up to index n, if necessary. */
    if (cache_len <= n) {
        idx_t required = n + 1;
        if (required > cache_cap) {
            size_t new_cap = (size_t)cache_cap ? (size_t)cache_cap * 2 : 256;
            if (new_cap < required) new_cap = required;
            seq_t *tmp = seq_realloc(digit_cache, (size_t)cache_cap, new_cap);
            if (!tmp) abort();          /* allocation failure */
            digit_cache = tmp;
            cache_cap   = (idx_t)new_cap;
        }
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t digit = (rem_val * 10) / 249;
            rem_val = (rem_val * 10) % 249;
            digit_cache[i] = digit;
        }
        cache_len = required;
    }
    return digit_cache[n];
}
