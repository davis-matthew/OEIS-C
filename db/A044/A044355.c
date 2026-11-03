/*
 * A044355  Numbers n such that string 2,3 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for numbers whose decimal representation contains the substring
   "23" but whose predecessor does not, and caches the results for later
   calls.  The function works for any seq_t that supports the required
   arithmetic operations. */
#include <stdbool.h>
#include <stddef.h>
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

static bool has23(seq_t v)
{
    seq_t prev = 10;                 /* sentinel different from any digit 0–9 */
    while (v > 0)
    {
        seq_t curr = v % 10;
        v /= 10;
        if (prev == 3 && curr == 2)
            return true;
        prev = curr;
    }
    return false;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t cur_checked = 1;       /* next candidate to examine */

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset starts at 1; undefined for 0, return 0 */
        return (seq_t)0;

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Find the next qualifying number. */
        while (true)
        {
            if (has23(cur_checked) && !has23(cur_checked - 1))
            {
                /* Ensure sufficient capacity. */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cur_checked;
                cur_checked++; /* advance for the next search */
                break;
            }
            cur_checked++;
        }
    }
    return cache[n - 1];
}
