/*
 * A044759  Numbers n such that string 4,6 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_sub46
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044759 – numbers n such that the decimal string "46" occurs in n but not in n+1.
   Computed by brute‑force search. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_sub46.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    grow_cache((size_t)n);         /* ensure capacity for at least n terms */

    while (cache_size < (size_t)n)
    {
        seq_t candidate = (cache_size == 0) ? 46 : cache[cache_size - 1] + 1;
        while (true)
        {
            seq_t next = candidate + 1;
            if (has_sub46(candidate) && !has_sub46(next))
                break;
            candidate = candidate + 1;
        }
        cache[cache_size] = candidate;
        ++cache_size;
    }
    return cache[n - 1];
}
