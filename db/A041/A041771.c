/*
 * A041771  Denominators of continued fraction convergents to sqrt(406).
 * https://oeis.org/A041771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 196 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
// A041771: Denominators of continued fraction convergents to sqrt(406)
// Formula (for n > 27):
//     a(n) = 118936190 * a(n-14) - a(n-28)
// Initial values a(0)..a(27) are generated from the continued fraction
// of sqrt(406) (length‑14 period: 20; 6,1,2,4,7,1,4,1,7,4,2,1,6,40).
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

/* compute the first 28 terms from the continued fraction of sqrt(406) */
static void init_terms(seq_t init[28])
{
    /* m, d, and the periodic partial quotients are tiny; use 64‑bit */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_k = 20;          /* a0 */
    uint64_t a[28];
    a[0] = 20;

    /* generate the period (14 terms after a0) */
    for (int i = 1; i <= 14; ++i) {
        m = d * a_k - m;
        d = (406 - m * m) / d;
        a_k = (20 + m) / d;
        a[i] = a_k;
    }
    /* repeat the period to get up to index 27 */
    for (int i = 15; i < 28; ++i)
        a[i] = a[i - 14];

    /* compute the denominators q_k with q_{-2}=1, q_{-1}=0 */
    seq_t q_prev2 = 1;
    seq_t q_prev1 = 0;
    for (int k = 0; k < 28; ++k) {
        seq_t qk = (seq_t)a[k] * q_prev1 + q_prev2;
        init[k] = qk;
        q_prev2 = q_prev1;
        q_prev1 = qk;
    }
}

/* global cache for the sequence */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static bool cache_initialized = false;
static seq_t init_cache[28];

seq_t a(idx_t n)
{
    /* initialise the cache on first call */
    if (!cache_initialized) {
        init_terms(init_cache);
        cache_initialized = true;
    }

    /* grow the cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t required = (size_t)n + 1;
        if (required > cache_cap)
            grow_seq_array(&cache, &cache_cap, required);

        /* fill new entries */
        for (size_t i = cache_size; i < required; ++i) {
            if (i < 28) {
                cache[i] = init_cache[i];
            } else {
                /* recurrence: a(n) = 118936190*a(n-14) - a(n-28) */
                cache[i] = (seq_t)118936190 * cache[i - 14] - cache[i - 28];
            }
        }
        cache_size = required;
    }
    return cache[n];
}
