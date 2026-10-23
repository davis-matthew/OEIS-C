/*
 * A115817  Integers i such that 4*i XOR 13*i = 17*i.
 * https://oeis.org/A115817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A115817: integers i such that (4*i) XOR (13*i) = 17*i */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    /* Number of terms computed so far and last tested integer */
    static idx_t generated = 0;
    static idx_t next_i = 0;

    if (n < generated)
        return cache[n];

    /* Ensure the cache can hold at least n+1 terms */
    while (generated <= n) {
        seq_t v = (seq_t)next_i;
        seq_t a4 = v * (seq_t)4U;
        seq_t a13 = v * (seq_t)13U;
        if ((a4 & a13) == (seq_t)0) {
            /* Resize cache if needed */
            if (generated >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 8;
                while (new_cap <= generated)
                    new_cap <<= 1;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[generated] = v;
            generated++;
        }
        next_i++;
    }

    return cache[n];
}
