/*
 * A162811  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=25, a(2)=600, a(3)=14100;
   For n>=4: a(n) = 23*a(n-1) + 23*a(n-2) - 276*a(n-3). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* handle the first four indices directly */
    if (n < 4)
        return n == 0 ? (seq_t)1
              : n == 1 ? (seq_t)25
              : n == 2 ? (seq_t)600
              : (seq_t)14100;

    size_t idx    = (size_t)n;
    size_t needed = idx + 1;

    /* grow the cache if necessary */
    if (needed > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise the first four terms if not already done */
    if (len < 4) {
        if (len <= 0) cache[0] = (seq_t)1;
        if (len <= 1) cache[1] = (seq_t)25;
        if (len <= 2) cache[2] = (seq_t)600;
        if (len <= 3) cache[3] = (seq_t)14100;
        len = 4;
    }

    /* compute any missing terms up to idx */
    for (size_t i = len; i <= idx; ++i) {
        seq_t v = 23 * cache[i - 1] + 23 * cache[i - 2] - 276 * cache[i - 3];
        cache[i] = v;
    }
    len = needed;

    return cache[idx];
}
