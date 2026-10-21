/*
 * A074581  a(n) = T(3n+1), where T(n) are tribonacci numbers A000073.
 * https://oeis.org/A074581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*a(n-1) - 5*a(n-2) + a(n-3), with a(0)=0, a(1)=2,
 *            a(2)=13
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t used  = 0;

    /* initialise cache for first three terms if needed */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)13;
        used   = 3;
    }

    /* extend cache if n beyond current capacity */
    if (n >= cap) {
        size_t new_cap = cap > 0 ? cap * 2 : 16;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed – undefined behaviour expected not to occur */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values up to n */
    while (used <= n) {
        /* recurrence: a(n) = 7*a(n-1) - 5*a(n-2) + a(n-3) */
        seq_t v = 7 * cache[used - 1] - 5 * cache[used - 2] + cache[used - 3];
        cache[used++] = v;
    }

    return cache[n];
}
