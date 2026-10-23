/*
 * A120157  a(n) = 13 + floor(Sum_{j=1..n-1} a(j)/3).
 * https://oeis.org/A120157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 69, u64 n <= 146,
 *            u128 n <= 299, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A120157
 * Recurrence: a(1) = 13
 *              a(n) = 13 + floor( ( sum_{j=1}^{n-1} a(j) ) / 3 )   for n > 1
 * The function returns a(n) for any n ≥ 1; values that do not fit into
 * the supplied seq_t are not required to be correct.
 */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache of a(1)..a(cached_len) */
static idx_t  cached_len = 0;        /* number of cached terms            */
static idx_t  cached_cap = 0;        /* allocated capacity                 */

/* Sum of terms a(1)..a(cached_len) */
static unsigned __int128 sum128 = 0; /* for integral seq_t : 4‑byte/8‑byte types */
static seq_t       sum_big  = 0;     /* for arbitrary‑precision seq_t      */

/* Ensure cache has capacity for at least 'need' terms. */
static void ensure_capacity(idx_t need)
{
    if (cached_cap < need) {
        idx_t new_cap = cached_cap ? cached_cap << 1 : 1;
        while (new_cap < need) new_cap <<= 1;

        seq_t *new_arr = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (!new_arr) abort(); /* out of memory */

        if (cached_len > 0) {
            memcpy(new_arr, cache, sizeof(seq_t) * cached_len);
            free(cache);
        }
        cache = new_arr;
        cached_cap = new_cap;
    }
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1 */

    if (cached_len < n) {
        ensure_capacity(n);

        /* Append new terms up to n. */
        for (idx_t i = cached_len + 1; i <= n; ++i) {
            seq_t val;
            if (sizeof(seq_t) <= 8) {              /* integral seq_t    */
                val = (seq_t)13 + (seq_t)(sum128 / 3);
                sum128 += (unsigned __int128)val;
            } else {                               /* arbitrary‑precision */
                val = (seq_t)13 + sum_big / 3;
                sum_big += val;
            }
            cache[i - 1] = val;
        }
        cached_len = n;
    }
    return cache[n - 1];
}
