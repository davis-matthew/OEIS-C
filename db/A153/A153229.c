/*
 * A153229  a(0) = 0, a(1) = 1, and for n >= 2, a(n) = (n-1) * a(n-2) + (n-2)
 *          * a(n-1).
 * https://oeis.org/A153229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 195 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1,
   a(n) = (n‑1)*a(n‑2) + (n‑2)*a(n‑1)  for n>=2 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Use the provided helper for array growth (handles construction of
   arbitrary‑precision seq_t objects). */
#include <stdbool.h>

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

static seq_t *cache = NULL;   /* memoisation array */
static size_t cache_cap = 0;  /* capacity of the array */

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_cap(size_t needed)
{
    if (cache_cap >= needed) return;

    size_t new_cap = cache_cap ? cache_cap : 2;
    while (new_cap < needed) new_cap <<= 1;

    /* grow_seq_array constructs elements correctly for arbitrary‑precision
       seq_t. It updates both the pointer and the size. */
    grow_seq_array(&cache, &cache_cap, new_cap);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Make cache large enough and initialise the first two values. */
    ensure_cap((size_t)n + 1);
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term = (seq_t)(i - 1) * cache[i - 2]
                   + (seq_t)(i - 2) * cache[i - 1];
        cache[i] = term;
    }

    return cache[n];
}
