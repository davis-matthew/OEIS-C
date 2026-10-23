/*
 * A039161  Numbers whose base-11 representation has the same number of 0's
 *          and 6's.
 * https://oeis.org/A039161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A039161: numbers whose base‑11 representation has an equal
   number of 0's and 6's. 
   Computed by scanning successive integers and keeping those that satisfy
   the property. */
#include <stdlib.h>

/* static cache for the sequence */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;  /* next integer to test */

/* Ensure cache can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* fill cache until it contains at least n terms */
    while (cache_len < n) {
        idx_t val = next_candidate++;
        idx_t tmp = val;
        unsigned c0 = 0, c6 = 0;
        while (tmp > 0) {
            unsigned d = tmp % 11;
            if (d == 0) c0++;
            else if (d == 6) c6++;
            tmp /= 11;
        }
        if (c0 == c6) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)val;
        }
    }
    return cache[n - 1];
}
