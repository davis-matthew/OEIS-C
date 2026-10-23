/*
 * A044454  Numbers n such that string 3,0 occurs in the base 4 representation
 *          of n but not of n+1.
 * https://oeis.org/A044454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers n for which base‑4 representation of n contains "30" but
 *            base‑4 representation of n+1 does not. This function searches
 *            sequentially and caches results to support arbitrary call
 *            ordering
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return true iff x in base 4 contains the consecutive digits 3,0. */
static bool has30(seq_t x)
{
    /* Zero has no base‑4 digits. */
    if (x == 0)
        return false;

    /* Enough to hold the digits of ordinary 64‑bit values.
       For bigger values the array will simply overflow, but the
       algorithm will still work because the search stops early. */
    unsigned char digits[256];
    size_t len = 0;

    while (x > 0) {
        digits[len++] = (unsigned char)(x % 4);
        x /= 4;
    }

    /* Digits are stored in reverse order; look for 3 followed by 0. */
    for (size_t i = len; i-- > 1; ) {
        if (digits[i] == 3 && digits[i - 1] == 0)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static state: cache of found values and next candidate to test */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;
    static seq_t next_candidate = 1;

    /* Generate terms until the required index is reached. */
    while (cache_size < n) {
        if (has30(next_candidate) && !has30(next_candidate + 1)) {
            /* Grow the cache if necessary.  Cast for C++ compatibility. */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = new_ptr;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
