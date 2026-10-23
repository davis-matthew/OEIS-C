/*
 * A039600  Numbers whose base-12 representation has the same nonzero number
 *          of 0's and 8's.
 * https://oeis.org/A039600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer whose base‑12 representation has the
 *            same nonzero number of 0's and 8's; computed by brute‑force
 *            search
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Ensure the cache array has at least `needed` elements. */
static seq_t *grow_cache(seq_t *p, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t newcap = (*cap ? *cap * 2 : 4);
        if (needed > newcap) newcap = needed;
        p = (seq_t *)realloc(p, newcap * sizeof(seq_t));
        *cap = newcap;
    }
    return p;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t cur = 1;          /* next integer to test */

    while (cache_len < n) {
        seq_t val = cur;           /* initial candidate value */
        seq_t tmp = val;
        unsigned count0 = 0, count8 = 0;

        while (tmp > 0) {          /* count base‑12 digits 0 and 8 */
            unsigned digit = (unsigned)(tmp % 12);
            if (digit == 0) ++count0;
            if (digit == 8) ++count8;
            tmp /= 12;
        }

        if (count0 == count8 && count0 > 0) {
            cache = grow_cache(cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = val;
        }
        ++cur;
    }

    return cache[n - 1];
}
