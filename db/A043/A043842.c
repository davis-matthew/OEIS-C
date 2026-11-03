/*
 * A043842  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 5 mod 6.
 * https://oeis.org/A043842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose number of runs in its *
 *            base‑4 representation is congruent to 5 modulo 6
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Count the number of runs in the base‑4 representation of a 64‑bit
 * unsigned integer.  A run is a maximal block of consecutive equal
 * digits.  For example, 122341 in base 4 (which is 4141₂₀) has runs 1,22,3,4,1
 * → 5 runs. */
static inline unsigned char runs_in_base4_u64(uint64_t n)
{
    unsigned char runs = 1;              /* at least one run */
    int last = n & 3;                    /* n % 4 */
    n >>= 2;                             /* n /= 4 */
    while (n > 0) {
        int d = n & 3;
        if (d != last) {
            ++runs;
            last = d;
        }
        n >>= 2;
    }
    return runs;
}

/* Dynamic array of results.  Elements are stored as seq_t. */
static seq_t *cache = NULL;
static size_t cache_size = 0;          /* number of elements stored */
static size_t cache_cap  = 0;          /* allocated capacity */
static idx_t  last_checked = 0;        /* last integer examined */

/* Ensure capacity for at least one more element. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
    if (new_ptr == NULL) exit(EXIT_FAILURE); /* cannot proceed */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    /* Extend the cache until we have at least n elements. */
    while (cache_size < (size_t)n) {
        ++last_checked;                                   /* candidate integer */
        /* Use 64‑bit arithmetic for counting runs; safe up to
           the upper bound required for typical tests. */
        unsigned char runs = runs_in_base4_u64((uint64_t)last_checked);
        if ((runs % 6) == 5) {
            /* Store this integer as seq_t in the cache. */
            if (cache_size == cache_cap) grow_cache();
            cache[cache_size] = (seq_t)last_checked;
            ++cache_size;
        }
    }
    return cache[n - 1];   /* zero‑based array indexing */
}
