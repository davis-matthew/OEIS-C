/*
 * A210102  Number of (n+1) X 4 0..2 arrays with every 2 X 2 subblock having
 *          one or three distinct values, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A210102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) -46*a(n-2) -190*a(n-3) +523*a(n-4) +341*a(n-5)
 *            -1220*a(n-6) +184*a(n-7) +488*a(n-8) -96*a(n-9) , a(1)=135,
 *            a(2)=1690, a(3)=20687, a(4)=254314, a(5)=3125615, a(6)=38420586,
 *            a(7)=472285295, a(8)=5805666474, a(9)=71367850223
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* out of range */

    /* 0‑based index of the requested term */
    size_t idx = (size_t)n - 1;

    /* static memoisation array */
    static seq_t *cache = NULL;   /* cache[0] = a(1) */
    static size_t cap   = 0;      /* current allocation size  */
    static size_t sz    = 0;      /* number of terms actually computed */

    /* allocate initial cache if needed */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;                 /* allocation failure */
    }

    /* initialise first nine terms (counted from 1) */
    if (sz < 9) {
        static const seq_t init[9] = {
            (seq_t)135ULL,
            (seq_t)1690ULL,
            (seq_t)20687ULL,
            (seq_t)254314ULL,
            (seq_t)3125615ULL,
            (seq_t)38420586ULL,
            (seq_t)472285295ULL,
            (seq_t)5805666474ULL,
            (seq_t)71367850223ULL
        };
        size_t len = (sz < 9) ? 9 : sz;
        for (size_t i = sz; i < len; ++i)
            cache[i] = init[i];
        sz = len;
    }

    /* if the requested term is already known, return it */
    if (idx < sz)
        return cache[idx];

    /* grow cache if necessary */
    if (idx >= cap) {
        size_t new_cap = cap;
        while (new_cap <= idx)
            new_cap <<= 1;                 /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to idx using the recurrence */
    for (size_t i = sz; i <= idx; ++i) {
        /* i corresponds to a(i+1) */
        if (i < 9) {          /* already initialised */
            continue;
        }

        /* positive contributions */
        seq_t pos = (seq_t)0;
        pos += cache[i-1] * (seq_t)17;      /* a(n-1) */
        pos += cache[i-4] * (seq_t)523;     /* a(n-4) */
        pos += cache[i-5] * (seq_t)341;     /* a(n-5) */
        pos += cache[i-7] * (seq_t)184;     /* a(n-7) */
        pos += cache[i-8] * (seq_t)488;     /* a(n-8) */

        /* negative contributions */
        seq_t neg = (seq_t)0;
        neg += cache[i-2] * (seq_t)46;      /* a(n-2) */
        neg += cache[i-3] * (seq_t)190;     /* a(n-3) */
        neg += cache[i-6] * (seq_t)1220;    /* a(n-6) */
        neg += cache[i-9] * (seq_t)96;      /* a(n-9) */

        /* a(n) = pos - neg ;  subtraction on unsigned types wraps on unsigned overflow,
           but the recurrence guarantees pos >= neg for all n >= 1. */
        cache[i] = pos - neg;
    }

    sz = idx + 1;
    return cache[idx];
}
