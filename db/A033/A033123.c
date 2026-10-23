/*
 * A033123  Base-5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,1.
 * https://oeis.org/A033123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*a(n-1) + a(n-3) - 5*a(n-4) a(1)=1, a(2)=5, a(3)=26,
 *            a(4)=131 (sequence offset 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* out of offset */

    /* static cache */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t last_computed = 0;

    /* ensure cache can hold index n (array is 0‑based) */
    if (cache_size < (size_t)n + 1) {
        size_t new_size = cache_size ? cache_size * 2 : 16;
        while (new_size <= (size_t)n) new_size <<= 1;

        seq_t *new_cache = alloc_seq_array(new_size);
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }
        cache = new_cache;
        cache_size = new_size;
    }

    /* initialise base values if necessary */
    if (last_computed < 4) {
        if (last_computed < 1) cache[1] = (seq_t)1;
        if (last_computed < 2) cache[2] = (seq_t)5;
        if (last_computed < 3) cache[3] = (seq_t)26;
        if (last_computed < 4) cache[4] = (seq_t)131;
        last_computed = 4;
    }

    /* compute missing terms up to n */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        seq_t tmp = cache[i-1] * (seq_t)5;
        tmp += cache[i-3];
        tmp -= cache[i-4] * (seq_t)5;
        cache[i] = tmp;
    }
    last_computed = n;

    return cache[(size_t)n];
}
