/*
 * A044566  Numbers n such that string 6,5 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer x (n≥1) whose base‑7 representation
 *            contains the substring "65" while the base‑7 representation of
 *            x+1 does not
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_65_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_65_base7.h"

/* -------------------------- memoized search -------------------------- */

static seq_t *cache = NULL;   /* array of found terms */
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* current capacity of the array */
static seq_t next_candidate = 0; /* next number to test */

/* Grow the cache so it can hold at least 'needed' elements.             */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* -------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* defined only for n ≥ 1 */

    size_t target_index = (size_t)(n - 1);       /* 0‑based index in cache */

    /* enlarge cache if we already have enough terms */
    if (target_index < cache_len) return cache[target_index];

    grow_cache(target_index + 1);

    /* compute terms until we have at least n of them            */
    while (cache_len < n)
    {
        if (has_65_base7(next_candidate) &&
            !has_65_base7(next_candidate + 1))
        {
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }

    return cache[target_index];
}
