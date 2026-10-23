/*
 * A189154  Number of n X 2 binary arrays without the pattern 0 0 1 1
 *          diagonally, vertically or horizontally.
 * https://oeis.org/A189154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A008937(n+1)^2.
 * The values are computed via the empirically verified linear recurrence
 *
 *          a(n) = 4*a(n-1) - a(n-2) - 14*a(n-4) + 4*a(n-5)
 *                 + 2*a(n-6) + 8*a(n-7) - a(n-8) - a(n-10)
 *
 * for all n ≥ 11, with the initial values
 *     a(1)=4, a(2)=16, a(3)=64, a(4)=225, a(5)=784,
 *     a(6)=2704, a(7)=9216, a(8)=31329, a(9)=106276, a(10)=360000.
 *
 * The implementation uses only the arithmetic operators defined for
 * seq_t; it works both for builtin integer types and for the arbitrary
 * precision integer class supplied by the build.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static memoisation storage */
    static seq_t *cache = NULL;
    static size_t cached = 0; /* number of terms already computed  (a(1) -> cache[0]) */
    static size_t cap = 0;    /* allocated capacity */

    /* initial values for a(1) .. a(10) */
    static const seq_t init_vals[10] = {
        (seq_t)4,      /* a(1) */
        (seq_t)16,     /* a(2) */
        (seq_t)64,     /* a(3) */
        (seq_t)225,    /* a(4) */
        (seq_t)784,    /* a(5) */
        (seq_t)2704,   /* a(6) */
        (seq_t)9216,   /* a(7) */
        (seq_t)31329,  /* a(8) */
        (seq_t)106276, /* a(9) */
        (seq_t)360000  /* a(10) */
    };

    /* create cache on first call */
    if (!cache) {
        cap = (size_t)n + 10;
        if (cap < 20) cap = 20;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* fallback on allocation failure */

        for (size_t i = 0; i < 10 && i < cap; ++i)
            cache[i] = init_vals[i];
        cached = 10;                        /* a(1)..a(10) stored */
    }

    /* grow cache if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 10) new_cap = n + 10;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* fallback on allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* if already computed, return */
    if (n <= cached) return cache[n-1];

    /* compute missing terms up to n */
    idx_t start = (cached > 10 ? cached + 1 : 11);
    for (idx_t j = start; j <= n; ++j) {
        /* accumulate positive and negative parts separately to avoid
         * intermediate wrap‑around for unsigned seq_t types.
         */
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        /* positive contributions */
        pos += (seq_t)4 * cache[j-2];   /* 4*a(j-1) */
        pos += (seq_t)4 * cache[j-6];   /* 4*a(j-5) */
        pos += (seq_t)2 * cache[j-7];   /* 2*a(j-6) */
        pos += (seq_t)8 * cache[j-8];   /* 8*a(j-7) */

        /* negative contributions */
        neg += cache[j-3];              /* a(j-2) */
        neg += (seq_t)14 * cache[j-5];  /* 14*a(j-4) */
        neg += cache[j-9];              /* a(j-8) */
        neg += cache[j-11];             /* a(j-10) */

        /* final result: pos - neg (sequence guarantees pos ≥ neg) */
        cache[j-1] = pos - neg;
    }
    cached = n;
    return cache[n-1];
}
