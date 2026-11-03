/*
 * A037998  Numbers whose maximal base-9 run length is 3.
 * https://oeis.org/A037998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term (n ≥ 1) of OEIS A037998.
   The sequence is defined by a brute‑force search: a(n) is the
   nth integer whose base‑9 representation contains a maximal run of
   identical digits of exact length 3.  No closed formula is known. */

/* The code is valid C11 and C++17.  It uses only integer arithmetic
   and the standard library. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Check whether the maximal run of identical base‑9 digits of x is 3. */
static bool has_max_run_three(idx_t x)
{
    if (x == 0)
        return false;

    int max_run = 0;
    int curr_run = 0;
    int prev_digit = -1;
    while (x > 0) {
        int d = (int)(x % 9);          /* digit in base 9 */
        if (d == prev_digit) {
            ++curr_run;
        } else {
            curr_run = 1;
            prev_digit = d;
        }
        if (curr_run > max_run)
            max_run = curr_run;
        x /= 9;
    }
    return max_run == 3;
}

/* Cached sequence values.  They are stored in type seq_t. */
static seq_t *cache         = NULL; /* pointer to array of seq_t */
static size_t cache_size   = 0;      /* number of filled entries */
static size_t cache_cap    = 0;      /* allocated capacity */

/* Grow the cache to hold at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (needed > new_cap)
        new_cap = needed;
    seq_t *newp = mem_resize(seq_t, cache, new_cap);
    if (!newp) {
        abort(); /* allocation failed */
    }
    cache = newp;
    cache_cap = new_cap;
}

/* Next integer to test for the property.  Stored as idx_t to avoid
   overflow when seq_t is small. */
static idx_t next_candidate = 0;

/* Public entry point: returns the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* Fill the cache until we have at least n terms. */
    while (cache_size < (size_t)n) {
        ++next_candidate;
        if (has_max_run_three(next_candidate)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = (seq_t)next_candidate;
        }
    }
    return cache[(size_t)n - 1];
}
