/*
 * A170538  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A170538: number of reduced words of length n
   in the Coxeter group on 49 generators with (S_i)^2 = I,
   (S_i S_j)^46 = I.
   Recurrence (derived from the given generating function):
     a(0) = 1
     for 1 ≤ n ≤ 45    : a(n) = 47·(a(n‑1)+...+a(1)) + 2
     for n = 46        : a(46) = 47·(a(45)+...+a(1)) + 1 – 1128·a(0)
     for n ≥ 47        : a(n) = 47·(a(n‑1)+...+a(n‑45)) – 1128·a(n‑46)
*/

#include <stddef.h>                    /* size_t */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static seq_t *cache   = NULL;   /* cached terms */
static size_t cache_cap = 0;    /* allocated capacity */
static size_t cache_len = 0;    /* number of terms stored */

/* Ensure workspace big enough for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        /* grow_seq_array takes the required new length, not just new capacity */
        grow_seq_array(&cache, &cache_cap, (size_t)n + 1);
    }
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Initialise the first term if the cache is empty. */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    /* Compute any missing terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        for (size_t k = 1; k <= 45 && i >= k; ++k)
            sum += cache[i - k];

        seq_t val = sum * 47;   /* common part of all recurrences */

        if (i <= 45)
            val += 2;
        else if (i == 46)
            val += 1;

        if (i >= 46)
            val -= cache[i - 46] * 1128;

        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
