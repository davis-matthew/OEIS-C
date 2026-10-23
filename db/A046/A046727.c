/*
 * A046727  Related to Pythagorean triples: alternate terms of A001652 and
 *          A046090.
 * https://oeis.org/A046727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A046727
   Computed by the recurrence
        a(0) = 0,   a(1) = 3,   a(2) = 21
        a(n) = 6*a(n-1) - a(n-2) - 4*(-1)^{n-1}   (n ≥ 3)
   The recurrence uses only addition and subtraction, so it works
   uniformly for all integer sizes supported by seq_t. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;

    /* initialise cache with the first three values */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */

        cache[0] = (seq_t)0;       /* a(0) */
        cache[1] = (seq_t)3;       /* a(1) */
        cache[2] = (seq_t)21;      /* a(2) */
    }

    /* return already known value */
    if (n < cache_len)
        return cache[n];

    /* grow cache to hold terms up to n (inclusive) */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;          /* allocation failed */
    cache = tmp;
    cache_len = new_len;

    /* compute missing terms */
    for (idx_t i = 3; i <= n; ++i) {
        /* compute 6*a(i−1) using only additions */
        seq_t prev1 = cache[i-1];
        seq_t next = prev1;
        next += prev1;  /* 2× */
        next += prev1;  /* 3× */
        next += prev1;  /* 4× */
        next += prev1;  /* 5× */
        next += prev1;  /* 6× */

        next -= cache[i-2];   /* subtract a(i−2) */

        /* subtract or add 4 according to (-1)^{i-1} */
        if (((i-1) & 1) == 0)
            next -= (seq_t)4;   /* even exponent -> (-1)^{i-1} =  1 -> -4 */
        else
            next += (seq_t)4;   /* odd  exponent -> (-1)^{i-1} = -1 -> +4 */

        cache[i] = next;
    }

    return cache[n];
}
