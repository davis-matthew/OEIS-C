/*
 * A125842  a(1)=1. a(n) = the number of terms from among the first floor(n/2)
 *          terms of the sequence which are coprime to n.
 * https://oeis.org/A125842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ i | 1 ≤ i ≤ ⌊n/2⌋ , gcd(a(i),n)=1 }|, a(1)=1
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* One‑based cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size (including index 0) */
    static size_t cache_len = 0;   /* highest index computed */

    if (n < 1)
        return (seq_t)0;           /* undefined, harmless for n < 1 */

    /* Ensure the cache is large enough for indices up to n */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;        /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n if not already done */
    for (size_t idx = cache_len + 1; idx <= (size_t)n; ++idx) {
        if (idx == 1) {                /* base case */
            cache[1] = (seq_t)1;
            continue;
        }

        idx_t half = (idx >> 1);
        seq_t cnt  = (seq_t)0;
        seq_t n_seq = (seq_t)idx;

        for (idx_t j = 1; j <= half; ++j) {
            if (gcd(cache[(size_t)j], n_seq) == (seq_t)1)
                ++cnt;
        }
        cache[idx] = cnt;
    }

    cache_len = (size_t)n;
    return cache[(size_t)n];
}
