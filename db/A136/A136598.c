/*
 * A136598  Expansion of g.f. (2*x^3 + 5) / ( -x^5 + x^3 + 1).
 * https://oeis.org/A136598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-3) + a(n-5), a(0)=5, a(1)=0, a(2)=0, a(3)=-3, a(4)=0
 * Range:     i8 n <= 26, i16 n <= 57, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;          /* array of seq_t values */
    static size_t cache_len = 0;         /* current allocated length */
    static idx_t max_index = 4;          /* largest index that has been computed */

    if (cache == NULL) {                 /* one‑time initialisation */
        grow_seq_array(&cache, &cache_len, 5); /* allocate indices 0..4 */
        cache[0] = 5;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = -3;
        cache[4] = 0;
    }

    if (n <= max_index)                   /* already known */
        return cache[n];

    /* grow the cache if needed */
    if ((size_t)n + 1 > cache_len)
        grow_seq_array(&cache, &cache_len, (size_t)n + 1);

    /* compute new terms up to n */
    for (idx_t i = max_index + 1; i <= n; ++i)
        cache[i] = -cache[i - 3] + cache[i - 5];

    max_index = n;
    return cache[n];
}
