/*
 * A167710  a(n) = 10*2^n - 3*A083658(n+2).
 * https://oeis.org/A167710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 6*a(n-3), a(0)=1, a(1)=5, a(2)=13
 *            (This recurrence generates OEIS A167710.)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t filled = 0;   /* number of entries currently stored */

    /* Allocate or grow storage if first call */
    if (cap == 0) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Compute entries until we have a[n] */
    while (filled <= n) {
        /* Initial values */
        if (filled == 0) {
            cache[0] = (seq_t)1;
            filled = 1;
            continue;
        }
        if (filled == 1) {
            cache[1] = (seq_t)5;
            filled = 2;
            continue;
        }
        if (filled == 2) {
            cache[2] = (seq_t)13;
            filled = 3;
            continue;
        }

        /* Grow buffer if necessary */
        if (filled >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap   = new_cap;
        }

        /* Recurrence: a(n) = 2*a(n-1) + 3*a(n-2) - 6*a(n-3) */
        cache[filled] = 2 * cache[filled - 1] +
                        3 * cache[filled - 2] -
                        6 * cache[filled - 3];
        filled++;
    }

    return cache[n];
}
