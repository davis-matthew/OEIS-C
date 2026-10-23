/*
 * A199925  Number of sequences of n coin flips that win on the last flip, if
 *          the sequence of flips ends with (0,0,1,1).
 * https://oeis.org/A199925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-5) for n>7, initial values:
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=2, a(6)=4, a(7)=8
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Allocate array of seq_t, compatible with both C and C++ */
static seq_t *alloc_seq_array(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)malloc(n * sizeof(seq_t));
#endif
}

/* Reallocate array of seq_t, compatible with both C and C++ */
static seq_t *realloc_seq_array(seq_t *p, size_t old_cap, size_t new_cap)
{
#ifdef __cplusplus
    seq_t *q = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i) q[i] = p[i];
    delete[] p;
    return q;
#else
    return (seq_t*)realloc(p, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* static cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated capacity */
    static size_t max_n = 0;      /* highest n computed */

    /* first call: allocate cache and set initial values */
    if (!cache) {
        cap = 8;                 /* enough for n=7 */
        cache = alloc_seq_array(cap);
        if (!cache) return 0;    /* allocation failed */

        /* initial terms a(1)…a(7) */
        cache[0] = 0; /* a(1) */
        cache[1] = 0; /* a(2) */
        cache[2] = 0; /* a(3) */
        cache[3] = 1; /* a(4) */
        cache[4] = 2; /* a(5) */
        cache[5] = 4; /* a(6) */
        cache[6] = 8; /* a(7) */
        max_n = 7;
    }

    /* if requested term already computed, return it */
    if (n <= max_n) {
        return cache[n - 1];
    }

    /* grow cache if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        cache = realloc_seq_array(cache, cap, new_cap);
        if (!cache) return 0;    /* allocation failed */
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = max_n + 1; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = a(i-1)+a(i-2)+a(i-3)-a(i-5)  (i>7) */
        seq_t val = cache[i - 2] + cache[i - 3] + cache[i - 4] - cache[i - 6];
        cache[i - 1] = val;
    }
    max_n = (size_t)n;

    return cache[n - 1];
}
