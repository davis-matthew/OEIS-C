/*
 * A095797  Four-column array read by rows: T(n,k) for k=0..3 is the k-th
 *          component of the vector obtained by multiplying the n-th power of
 *          the 4 X 4 matrix (1,1,1,1; 7,3,1,0; 12,2,0,0; 6,0,0,0) and the
 *          vector (1,1,1,1).
 * https://oeis.org/A095797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = 4*a(n-4) + 24*a(n-8) - 30*a(n-12) - 12*a(n-16) for n >=
 *            16 Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=4,
 *            a(5)=11, a(6)=14, a(7)=6, a(8)=35, a(9)=75, a(10)=70, a(11)=24,
 *            a(12)=204, a(13)=540, a(14)=570, a(15)=210
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[16] = {
    (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
    (seq_t)4, (seq_t)11, (seq_t)14, (seq_t)6,
    (seq_t)35, (seq_t)75, (seq_t)70, (seq_t)24,
    (seq_t)204, (seq_t)540, (seq_t)570, (seq_t)210
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure the cache contains values up to n */
    if (n + 1 > (idx_t)cache_size) {
        /* Determine new cache size */
        size_t new_size = (size_t)(n + 1);
        /* Reallocate cache */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* Handle allocation failure gracefully */
            exit(EXIT_FAILURE);
        }
        cache = tmp;

        /* If this is the first allocation, fill initial values */
        if (cache_size == 0) {
            for (size_t i = 0; i < 16 && i < new_size; ++i)
                cache[i] = init_vals[i];
            cache_size = 16;
        }

        /* Compute any new terms up to n */
        for (size_t i = (size_t)cache_size; i < new_size; ++i) {
            if (i < 16) {
                cache[i] = init_vals[i];
            } else {
                seq_t part1 = cache[i-4] * (seq_t)4;
                part1 += cache[i-8] * (seq_t)24;
                seq_t part2 = cache[i-12] * (seq_t)30;
                part2 += cache[i-16] * (seq_t)12;
                cache[i] = part1 - part2;   /* part1 >= part2 by definition */
            }
        }
        cache_size = (idx_t)new_size;
    }

    return cache[(size_t)n];
}
