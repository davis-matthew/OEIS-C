/*
 * A039029  Numbers whose base-6 representation has the same number of 4's and
 *          5's.
 * https://oeis.org/A039029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9052 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by brute‑force search for numbers whose base‑6 representation
   contains an equal number of 4’s and 5’s.  The sequence is built lazily
   and cached for repeated calls. */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/*-----------------------------------------------------------------------*/
static bool has_equal_4_and_5(seq_t val)
{
    idx_t n4 = 0, n5 = 0;
    seq_t x = val;

    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)6;
        if (d == (seq_t)4) {
            ++n4;
        } else if (d == (seq_t)5) {
            ++n5;
        }
        x /= (seq_t)6;
    }
    return n4 == n5;          /* 0‑digit numbers also return true   */
}

/* Cached sequence values.  The cache grows lazily as larger n are requested. */
static seq_t *cache = NULL;           /* pointer to array of seq_t values      */
static size_t cache_len  = 0;         /* number of values stored in the cache */
static size_t cache_cap  = 0;         /* current capacity of the cache array  */
static seq_t next_val = 0;            /* next integer to test for the property */

/*-----------------------------------------------------------------------*/
/* Count the number of 4’s and 5’s in the base‑6 representation of x.   */

/*-----------------------------------------------------------------------*/
/* Grow the cache until it contains at least n elements.                */
/*-----------------------------------------------------------------------*/
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) {
        return;               /* already enough                  */
    }

    /* Determine the required capacity. */
    size_t needed = (size_t)n;

    if (cache == NULL) {
        cache_cap = needed > (size_t)128 ? needed : (size_t)128;
        cache     = mem_new(seq_t, cache_cap);
    } else if (cache_cap < needed) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (tmp == NULL) {
            /* In a real project we would handle out‑of‑memory.  For the
               purposes of the unit tests we simply abort. */
            abort();
        }
        cache       = tmp;
        cache_cap  = new_cap;
    }

    /* Find and store the next qualifying numbers. */
    for (; cache_len < needed; ++cache_len) {
        while (!has_equal_4_and_5(next_val)) {
            next_val = next_val + (seq_t)1;
        }
        cache[cache_len] = next_val;
        next_val = next_val + (seq_t)1;
    }
}

/*-----------------------------------------------------------------------*/
/* Public sequence function.  a(1) is the first term listed in OEIS.     */
/*-----------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;       /* out‑of‑range request      */
    }
    ensure_cache(n);
    return cache[n - 1];
}
