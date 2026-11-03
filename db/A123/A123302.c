/*
 * A123302  a(0) = 1. a(n) = the n-th integer from among those positive
 *          integers coprime to a(n-1).
 * https://oeis.org/A123302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 68 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; for n > 0, a(n) is the n‑th positive integer
   that is coprime to a(n‑1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for 0 <= i < cached_len */
    static seq_t *cache = NULL;
    static idx_t  cached_len = 0;   /* number of terms computed so far */
    static size_t cached_cap = 0;   /* allocated capacity of cache */

    /* if we already have the term, return it */
    if (n < cached_len)
        return cache[n];

    /* ensure there is space for the new terms */
    idx_t need = n + 1;
    if ((size_t)need > cached_cap)
        grow_seq_array(&cache, &cached_cap, (size_t)need);

    /* compute missing terms sequentially */
    for (idx_t i = cached_len; i < need; ++i)
    {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else {
            seq_t prev   = cache[i - 1];
            idx_t count  = 0;
            idx_t cand   = 0;
            /* the i-th coprime of prev (i starts from 1) */
            while (count < i) {
                ++cand;
                if (gcd(prev, (seq_t)cand) == (seq_t)1)
                    ++count;
            }
            cache[i] = (seq_t)cand;
        }
    }
    cached_len = need;
    return cache[n];
}
