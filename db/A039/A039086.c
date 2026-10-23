/*
 * A039086  Numbers whose base-9 representation has the same number of 1's and
 *          4's.
 * https://oeis.org/A039086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑9 representation has equal number of
   digit 1 and digit 4.  The function returns the n‑th such value
   (a(1) = 0).  It builds a static cache of all qualifying numbers
   up to the requested index.  The cache is grown by realloc.  */
#include <stdlib.h>   /* malloc, realloc, abort */

/* Helper to grow the cache when needed.  */
static void grow_cache(size_t cap_needed, seq_t **cache, size_t *cap)
{
    if (*cap >= cap_needed) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 16;
    if (new_cap < cap_needed) new_cap = cap_needed;
    seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* memory exhaustion */
    *cache = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Static state for memoization */
    static seq_t *cache       = NULL;
    static size_t len         = 0;
    static size_t cap         = 0;
    static idx_t last_checked = 0;   /* next integer to examine */

    /* Generate entries until we have at least n values */
    while (len < n)
    {
        idx_t x = last_checked;
        unsigned int c1 = 0, c4 = 0;

        if (x == 0) {
            /* 0 has no 1's and no 4's – counts are equal */
            c1 = c4 = 0;
        } else {
            while (x > 0) {
                uint8_t digit = x % 9U;
                if (digit == 1U) c1++;
                else if (digit == 4U) c4++;
                x /= 9U;
            }
        }

        if (c1 == c4) {
            grow_cache(len + 1, &cache, &cap);
            cache[len++] = (seq_t)last_checked;
        }

        last_checked++;
    }

    return cache[n - 1];
}
