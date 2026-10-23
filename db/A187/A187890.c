/*
 * A187890  a(1) = 0, a(2) = 4, a(n) = a(n-1) + a(n-2) - 1.
 * https://oeis.org/A187890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - 1, with a(1)=0, a(2)=4
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the nth term of OEIS sequence A187890.
 * The sequence is defined for n >= 1:
 *   a(1) = 0,  a(2) = 4,
 *   a(n) = a(n-1) + a(n-2) - 1  for n > 2.
 *  The implementation memoizes computed values in a static array
 *  so that subsequent calls in any order are O(1) after the first
 *  required expansion, and all calculations use only integer
 *  arithmetic on type seq_t. */
seq_t a(idx_t n)
{
    /* static cache: 1-indexed, cache[0] unused */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* largest n cached */

    if (n == 0)     /* undefined but safe to return 0 */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)4;

    /* expand cache if needed */
    if (n > cache_size) {
        idx_t new_size = n;
        size_t alloc_bytes = (size_t)(new_size + 1) * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, alloc_bytes);
        if (!tmp)          /* allocation failed – abort with 0 */
            return (seq_t)0;
        cache = tmp;

        /* initialise newly allocated area */
        idx_t start = cache_size + 1;
        if (start <= 1) {            /* first element not yet set */
            cache[1] = (seq_t)0;
            start = 2;
        }
        if (start <= 2) {            /* second element not yet set */
            cache[2] = (seq_t)4;
            start = 3;
        }

        /* compute missing terms */
        for (idx_t i = start; i <= new_size; ++i) {
            cache[i] = cache[i-1] + cache[i-2] - (seq_t)1;
        }
        cache_size = new_size;
    }

    return cache[n];
}
