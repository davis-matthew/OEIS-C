/*
 * A044659  Numbers n such that string 3,0 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044659
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

/* Sequence A044659 – numbers n whose base‑9 representation contains the
   substring "30" while the base‑9 representation of n+1 does not.
   The algorithm tests successive integers and caches found terms,
   so subsequent calls (even out‑of‑order) are answered in O(1). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return 1 iff the base‑9 representation of x contains the pattern 3,0. */
static int has_30_base9(uint64_t x)
{
    uint8_t digits[32];
    int len = 0;

    if (x == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (x != 0) {
            digits[len++] = (uint8_t)(x % 9);
            x /= 9;
        }
    }

    for (int i = len - 1; i >= 1; --i)
        if (digits[i] == 3 && digits[i - 1] == 0)
            return 1;
    return 0;
}

/* Cached sequence values.  They are stored in the type seq_t which may be
   a primitive unsigned integer or an arbitrary‑precision class. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements currently stored */
static size_t cache_cap = 0;   /* allocated capacity */
static uint64_t next_candidate = 1; /* next integer to test */

/* Ensures the cache has room for at least one more element. */
static void grow_cache(void)
{
    if (cache_cap == 0)
        cache_cap = 16;
    else
        cache_cap *= 2;
    /* Re‑allocate with an explicit cast, good for both C and C++. */
    cache = mem_resize(seq_t, cache, cache_cap);
}

/* a(idx_t n) – returns the n‑th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    /* Compute terms until we have at least n stored. */
    while (cache_len < (size_t)n) {
        uint64_t x = next_candidate;              /* integer to test   */
        if (has_30_base9(x) && !has_30_base9(x + 1)) {
            if (cache_len >= cache_cap)
                grow_cache();
            cache[cache_len++] = (seq_t)x;         /* store found term */
        }
        ++next_candidate;
    }
    return cache[n - 1]; /* direct indexed access */
}
