/*
 * A222868  Number of n X 2 0..5 arrays with no element equal to another at a
 *          city block distance of exactly two, and new values 0..5 introduced
 *          in row major order.
 * https://oeis.org/A222868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A222868
    Formula: a(n) = 21*a(n-1) - 84*a(n-2) + 64*a(n-3)   for n > 5
    Base values: a(1)=2, a(2)=7, a(3)=34, a(4)=370, a(5)=5314
    Memoised for arbitrary call order. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle out‑of‑range index */
    if (n == 0) return 0;

    /* Static state for memoisation */
    static idx_t max_n = 0;
    static seq_t *cache = NULL;
    static size_t capacity = 0;

    /* Ensure we have a cache big enough for requested n */
    if (n > max_n)
    {
        /* Extend capacity if necessary */
        if (capacity < (size_t)n)
        {
            size_t new_cap = capacity ? capacity << 1 : 16;
            while (new_cap < (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failed */
            cache = tmp;
            capacity = new_cap;
        }

        /* Initialise the first five values if they are not yet set */
        if (max_n < 1) {
            cache[0] = (seq_t)2;
            max_n = 1;
        }
        if (max_n < 2) {
            cache[1] = (seq_t)7;
            max_n = 2;
        }
        if (max_n < 3) {
            cache[2] = (seq_t)34;
            max_n = 3;
        }
        if (max_n < 4) {
            cache[3] = (seq_t)370;
            max_n = 4;
        }
        if (max_n < 5) {
            cache[4] = (seq_t)5314;
            max_n = 5;
        }

        /* Compute new terms using the recurrence */
        for (size_t k = max_n + 1; k <= (size_t)n; ++k)
        {
            /* k is 1‑based; cache indices are 0‑based: */
            /* a(k) = 21*a(k-1) - 84*a(k-2) + 64*a(k-3) */
            seq_t term = (seq_t)21 * cache[k-2]   /* 21*a(k-1) */
                       + (seq_t)64 * cache[k-4];/* +64*a(k-3)*/
            term -= (seq_t)84 * cache[k-3];    /* -84*a(k-2) */
            cache[k-1] = term;
        }
        max_n = n;
    }
    return cache[n-1];
}
