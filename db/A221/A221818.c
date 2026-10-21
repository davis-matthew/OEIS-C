/*
 * A221818  Number of 2 X n arrays of occupancy after each element moves to
 *          some horizontal, diagonal or antidiagonal neighbor, without
 *          move-in move-out straight through or left turns.
 * https://oeis.org/A221818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 54,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A221818
 *  Sequence recurrence:
 *  a(n) = a(n-1) + 17*a(n-2) + 25*a(n-3) + 7*a(n-4) + 15*a(n-5)
 *         + 10*a(n-6) - 19*a(n-7) + 8*a(n-8) + 9*a(n-9)
 *         + 2*a(n-10) - 3*a(n-11) + a(n-12)
 *  for n >= 13  (first 12 terms are defined below)
 *
 *  Implementation uses memoised linear recurrence; works for any
 *  seq_t that supports +, -, *, / and a mul_small helper.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* first 12 terms (a(1) .. a(12)) */
    static const seq_t init[12] = {
        0ULL, 4ULL, 16ULL, 91ULL, 459ULL, 2429ULL,
        12678ULL, 66380ULL, 347305ULL, 1817234ULL,
        9509123ULL, 49756006ULL
    };

    if (n == 0)
        return 0;                 /* out of range – return 0 */

    /* convert n to unsigned for array indices */
    size_t un = (size_t)n;

    /* initialise memoisation cache lazily */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    if (cache == NULL) {
        cap = 12;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
    } else if (un > cap) {
        /* grow cache to hold up to index n-1 */
        size_t new_cap = cap * 2;
        if (un > new_cap)
            new_cap = un;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();              /* memory exhausted – terminate */
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms on demand */
    for (size_t i = 12; i < un; ++i) {  /* i = index in cache, a(i+1) */
        seq_t term = cache[i - 1];                     /* a(k-1) */
        term += mul_small(cache[i - 2], 17);           /* 17*a(k-2) */
        term += mul_small(cache[i - 3], 25);           /* 25*a(k-3) */
        term += mul_small(cache[i - 4], 7);            /* 7*a(k-4) */
        term += mul_small(cache[i - 5], 15);           /* 15*a(k-5) */
        term += mul_small(cache[i - 6], 10);           /* 10*a(k-6) */
        term += mul_small(cache[i - 8], 8);            /* 8*a(k-8) */
        term += mul_small(cache[i - 9], 9);            /* 9*a(k-9) */
        term += mul_small(cache[i -10], 2);            /* 2*a(k-10) */
        term += cache[i - 12];                         /* a(k-12) */

        seq_t neg = mul_small(cache[i - 7], 19);       /* 19*a(k-7) */
        neg += mul_small(cache[i -11], 3);             /* 3*a(k-11) */

        cache[i] = term - neg;
    }

    return cache[un - 1];
}
