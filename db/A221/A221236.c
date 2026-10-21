/*
 * A221236  Number of n X 2 arrays of occupancy after each element moves to
 *          some horizontal, vertical or antidiagonal neighbor, without
 *          2-loops.
 * https://oeis.org/A221236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 23*a(n-2) - a(n-3) + 10*a(n-4) - a(n-6);
 *            a(1)=0, a(2)=5, a(3)=53, a(4)=467, a(5)=3913, a(6)=32299
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;      /* 1‑based cache, index 0 unused          */
static idx_t   a_cap   = 0;        /* allocated size (number of elements)    */
static idx_t   a_next  = 7;        /* next index to compute                  */

seq_t a(idx_t n)
{
    if (n <= 6) {
        static const seq_t init[7] = {0, 0, 5, 53, 467, 3913, 32299};
        return init[n];
    }

    /* --- allocate or grow cache if necessary --- */
    if (a_cache == NULL) {                  /* first allocation */
        idx_t cap = 16;
        while (cap <= n) cap <<= 1;
        a_cache = (seq_t *)malloc(cap * sizeof(seq_t));
        a_cap   = cap;
        static const seq_t init[7] = {0, 0, 5, 53, 467, 3913, 32299};
        for (idx_t i = 1; i <= 6; ++i) a_cache[i] = init[i];
        a_next = 7;
    } else if (a_cap <= n) {                /* growth */
        idx_t cap = a_cap << 1;
        while (cap <= n) cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, cap * sizeof(seq_t));
        a_cap   = cap;
    }

    /* --- compute missing terms up to n --- */
    for (; a_next <= n; ++a_next) {
        seq_t val = (seq_t)11 * a_cache[a_next-1];
        val -= (seq_t)23 * a_cache[a_next-2];
        val -=           a_cache[a_next-3];
        val += (seq_t)10 * a_cache[a_next-4];
        val -=           a_cache[a_next-6];
        a_cache[a_next] = val;
    }

    return a_cache[n];
}
