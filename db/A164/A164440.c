/*
 * A164440  Number of binary strings of length n with no substrings equal to
 *          0000 0110 or 1011.
 * https://oeis.org/A164440
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) with initial
 *            values: a(4) = 13, a(5) = 22, a(6) = 37, a(7) = 62, a(8) = 104
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 85, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* compute only for n >= 4; below the offset behaviour is irrelevant */
    if (n < 4)
        return 0;

    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored (indices 4 .. 4+size-1) */

    /* compute up to n if necessary */
    size_t needed = (size_t)(n - 3);  /* because n=4 needs 1 element */
    if (needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)
            return 0;  /* allocation failed – return 0 as a graceful fallback */
        cache = tmp;
        /* initialise new slots to 0 (optional, but safe) */
        for (size_t i = cache_size; i < needed; ++i)
            cache[i] = 0;
        /* fill initial values up to index n */
        for (size_t pos = cache_size; pos < needed; ++pos) {
            idx_t idx = (idx_t)pos + 4;  /* map array index to sequence index */
            if (idx == 4)
                cache[pos] = (seq_t)13;
            else if (idx == 5)
                cache[pos] = (seq_t)22;
            else if (idx == 6)
                cache[pos] = (seq_t)37;
            else if (idx == 7)
                cache[pos] = (seq_t)62;
            else if (idx == 8)
                cache[pos] = (seq_t)104;
            else {   /* recurrence for idx >= 9 */
                /* pos >= 5 for idx >= 9 */
                cache[pos] = 2 * cache[pos-1] - cache[pos-2]
                               + cache[pos-3] - cache[pos-4] + cache[pos-5];
            }
        }
        cache_size = needed;
    }

    return cache[n - 4];
}
