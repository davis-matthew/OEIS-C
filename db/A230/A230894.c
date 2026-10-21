/*
 * A230894  Number of nX3 0..3 arrays x(i,j) with each element horizontally or
 *          vertically next to at least one element with value (x(i,j)+1) mod
 *          4 and at least one element with value (x(i,j)-1) mod 4, no
 *          adjacent elements equal, and upper left element zero.
 * https://oeis.org/A230894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence (valid for n >= 12) a(n) = 5*a(n-1) +
 *            3*a(n-2) - 25*a(n-3) - 8*a(n-4) + 82*a(n-5) - 8*a(n-6) -
 *            132*a(n-7) + 80*a(n-8) + 12*a(n-9) - 32*a(n-10) + 16*a(n-11)
 *            Initial values: a(1)=0, a(2)=2, a(3)=14, a(4)=34, a(5)=198,
 *            a(6)=810, a(7)=3706, a(8)=16918, a(9)=76318, a(10)=347442, a...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* return 0 for n <= 0 (undefined behaviour for the sequence) */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;   /* array holding computed terms */
    static size_t cache_size = 0; /* number of elements currently stored */

    /* initial 11 terms of the sequence */
    static const seq_t init[11] = {
        (seq_t)0,
        (seq_t)2,
        (seq_t)14,
        (seq_t)34,
        (seq_t)198,
        (seq_t)810,
        (seq_t)3706,
        (seq_t)16918,
        (seq_t)76318,
        (seq_t)347442,
        (seq_t)1575090
    };

    /* lazily initialise the cache on the first call */
    if (cache_size == 0) {
        cache_size = 11;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            return (seq_t)0; /* allocation failed, return 0 as a safe fallback */
        for (size_t i = 0; i < 11; ++i)
            cache[i] = init[i];
    }

    /* return early for the first 11 known terms */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* extend the cache using the recurrence until it contains a(n) */
    while (cache_size < (size_t)n) {
        /* index i refers to the term we are about to compute, i.e. a(i+1) */
        size_t i = cache_size;           /* zero‑based index of next entry */
        seq_t a1  = cache[i - 1];   /* a(i)   */
        seq_t a2  = cache[i - 2];   /* a(i-1) */
        seq_t a3  = cache[i - 3];   /* a(i-2) */
        seq_t a4  = cache[i - 4];   /* a(i-3) */
        seq_t a5  = cache[i - 5];   /* a(i-4) */
        seq_t a6  = cache[i - 6];   /* a(i-5) */
        seq_t a7  = cache[i - 7];   /* a(i-6) */
        seq_t a8  = cache[i - 8];   /* a(i-7) */
        seq_t a9  = cache[i - 9];   /* a(i-8) */
        seq_t a10 = cache[i -10];   /* a(i-9) */
        seq_t a11 = cache[i -11];   /* a(i-10) */

        /* compute the next term by the empirical recurrence */
        seq_t term_add = (seq_t)5  * a1
                       + (seq_t)3  * a2
                       + (seq_t)82 * a5
                       + (seq_t)80 * a8
                       + (seq_t)12 * a9
                       + (seq_t)16 * a11;

        seq_t term_sub = (seq_t)25 * a3
                       + (seq_t)8  * a4
                       + (seq_t)8  * a6
                       + (seq_t)132 * a7
                       + (seq_t)32 * a10;

        seq_t next = term_add - term_sub;

        /* grow the cache array */
        seq_t *tmp = (seq_t *)realloc(cache, (cache_size + 1) * sizeof(seq_t));
        if (!tmp) {
            free(cache);          /* clean up before returning */
            cache = NULL;
            cache_size = 0;
            return (seq_t)0;      /* allocation failed */
        }
        cache = tmp;
        cache[cache_size] = next;
        cache_size++;
    }

    return cache[n - 1];
}
