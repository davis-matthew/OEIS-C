/*
 * A164495  Number of binary strings of length n with no substrings equal to
 *          0010 0101 or 1011.
 * https://oeis.org/A164495
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-4) + a(n-6) (for n ≥ 10) Initial
 *            values a(4) = 13, a(5) = 22, a(6) = 37, a(7) = 62, a(8) = 105,
 *            a(9) = 178 (Values for 0 ≤ n < 4 are irrelevant.)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 83, u128 n <= 168,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cached_upto = 0;
    static size_t cap = 0;

    /* ensure base values are in the cache */
    if (!cache) {
        /* allocate space for at least indices 0..9 */
        grow_seq_array(&cache, &cap, 10);
        cache[0] = cache[1] = cache[2] = cache[3] = 0;
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)22;
        cache[6] = (seq_t)37;
        cache[7] = (seq_t)62;
        cache[8] = (seq_t)105;
        cache[9] = (seq_t)178;
        cached_upto = 9;
    }

    if (n <= cached_upto)
        return cache[n];

    /* grow cache to n */
    grow_seq_array(&cache, &cap, n + 1);

    /* compute terms up to n */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
        if (i >= 10) {
            /* recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-4) + a(n-6) */
            seq_t val = 2 * cache[i - 1] - cache[i - 2] + cache[i - 4] + cache[i - 6];
            cache[i] = val;
        } else {
            /* i in [4,9] already initialized */
        }
    }
    cached_upto = n;
    return cache[n];
}
