/*
 * A058094  Number of 321-hexagon-avoiding permutations in S_n, i.e.,
 *          permutations of 1..n with no submatrix equivalent to 321,
 *          56781234, 46781235, 56718234 or 46718235.
 * https://oeis.org/A058094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 9*a(n-3) - 4*a(n-4) - 4*a(n-5) +
 *            a(n-6) for n >= 6, with a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=14,
 *            a(5)=42
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n < cache_len)
        return cache[n];

    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {            /* allocation failed */
        return (seq_t)0;
    }
    cache = tmp;

    for (idx_t i = cache_len; i < new_len; ++i) {
        if (i == 0)           cache[i] = (seq_t)1;
        else if (i == 1)      cache[i] = (seq_t)1;
        else if (i == 2)      cache[i] = (seq_t)2;
        else if (i == 3)      cache[i] = (seq_t)5;
        else if (i == 4)      cache[i] = (seq_t)14;
        else if (i == 5)      cache[i] = (seq_t)42;
        else
            cache[i] = ( seq_t )( 6 * cache[i - 1]
                                 - 11 * cache[i - 2]
                                 + 9  * cache[i - 3]
                                 - 4  * cache[i - 4]
                                 - 4  * cache[i - 5]
                                 +   cache[i - 6] );
    }

    cache_len = new_len;
    return cache[n];
}
