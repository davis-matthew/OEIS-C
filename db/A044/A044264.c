/*
 * A044264  Numbers n such that string 1,4 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that the base‑9 representation of n contains the substring
 * “1,4” (i.e. a digit 1 immediately followed by a digit 4) whereas n‑1 does
 * not.  This function returns the n‑th element of that sequence (n ≥ 1).
 * The implementation builds the sequence incrementally and memoizes all
 * previously computed terms.  It works for all supported seq_t and idx_t
 * types (including arbitrary‑precision seq_t classes in C++).
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

/* Return true if the base‑9 representation of v contains the adjacent digits
 * 1 followed by 4.  v is an idx_t (uint64_t). */
static bool has_14_in_base9(idx_t v)
{
    int prev = -1;                 /* lower digit, -1 means none yet */
    while (v > 0) {
        int d = v % 9;             /* current higher digit */
        if (d == 1 && prev == 4)
            return true;
        prev = d;
        v /= 9;
    }
    return false;
}

/* Memoisation data ------------------------------------------------------- */
static seq_t *cache = NULL;        /* array of found terms */
static size_t cache_size = 0;      /* number of terms stored */
static size_t cache_cap   = 0;     /* allocated capacity */
static idx_t  last_checked = 0;    /* largest integer already examined */

/* Grow the storage array so that it can hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_ptr = alloc_seq_array(new_cap);
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            new_ptr[i] = cache[i];
        free_seq_array(cache);
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n < 1 */

    /* Ensure the cache has at least 'n' terms. */
    while (cache_size < (size_t)n) {
        last_checked += 1;               /* next integer to examine */

        if (has_14_in_base9(last_checked) &&
            !has_14_in_base9(last_checked - 1)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = (seq_t)last_checked;
        }
    }

    return cache[(size_t)n - 1];
}
