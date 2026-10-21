/*
 * A221304  Number of n X 2 arrays of occupancy after each element moves to
 *          some horizontal, vertical or antidiagonal neighbor, with every
 *          occupancy equal to zero or two.
 * https://oeis.org/A221304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 2*a(n-4) + a(n-6) * offset 1,
 *            initial values: a(1)=0, a(2)=5, a(3)=10, a(4)=31, a(5)=90,
 *            a(6)=259
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1 */

    /* --------- dynamic cache allocation that works in C and C++ --------- */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;
    static idx_t    computed_up_to = 0;

    /* initial allocation */
    if (cache == NULL) {
        size_t init = 64;
        #ifdef __cplusplus
            cache = new seq_t[init];
        #else
            cache = (seq_t*)malloc(init * sizeof(seq_t));
        #endif
        cap = init;
    }

    /* enlarge if needed */
    if ((idx_t)n + 1 > cap) {
        size_t newcap = cap;
        while ((idx_t)newcap <= n)
            newcap <<= 1;
        #ifdef __cplusplus
            seq_t *tmp = new seq_t[newcap];
            for (idx_t i = 0; i < cap; ++i)
                tmp[i] = cache[i];
            delete [] cache;
        #else
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        #endif
        cache = tmp;
        cap   = newcap;
    }

    /* base values (filled once) */
    static const seq_t base[7] = {0, 0, 5, 10, 31, 90, 259};

    if (computed_up_to < 6) {
        for (idx_t i = 1; i <= 6; ++i)
            cache[i] = base[i];
        computed_up_to = 6;
    }

    /* recurrence for n > 6 */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t val = (seq_t)4 * cache[i - 1]
                  - (seq_t)3 * cache[i - 2]
                  - (seq_t)2 * cache[i - 4]
                  +           cache[i - 6];
        cache[i] = val;
    }
    if (n > computed_up_to)
        computed_up_to = n;

    return cache[n];
}
