/*
 * A239453  Convolution of the generalized Catalan numbers A057977 with
 *          themselves.
 * https://oeis.org/A239453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 15, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A239453
   Convolution of the generalized Catalan numbers A057977 with themselves.
   The sequence satisfies the linear recurrence
        -(n+4)*(137*n-1035)*a(n)
        +(383*n^2-1622*n-5421)*a(n-1)
        +4*(274*n^2-1385*n-3534)*a(n-2)
        +4*(-766*n^2+4393*n+555)*a(n-3)
        +16*(-137*n^2+898*n-606)*a(n-4)
        +16*(n-3)*(383*n-1622)*a(n-5)=0    for n≥5
   with initial values a(0)=1, a(1)=2, a(2)=3, a(3)=8, a(4)=11.
   The implementation below computes terms on demand and memoises all
   previously computed values.  It works for arbitrary‑precision seq_t
   (as a class in C++) and for unsigned integer seq_t (e.g. uint64_t)
   without using any floating‑point operations. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ensure that the cache and its “computed” flag array are large enough
   to hold indices up to 'need' (inclusive) */
static void ensure_cap(idx_t need, seq_t **arr, unsigned char **flag, size_t *cap)
{
    if (need + 1 <= *cap)
        return;
    size_t new_cap = (*cap > 0) ? (*cap) * 2 : 16;
    while (new_cap <= (size_t)need)
        new_cap <<= 1;
    *arr   = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    *flag  = (unsigned char*)realloc(*flag, new_cap * sizeof(unsigned char));
    *cap   = new_cap;
}

/* sequence value a(n) */
seq_t a(idx_t n)
{
    /* static storage for memoised terms and a flag whether the term
       has been computed */
    static seq_t *cache   = NULL;
    static unsigned char *computed = NULL;
    static size_t cap   = 0;
    static size_t last_computed = 0;      /* largest index computed so far */
    static bool initialized = false;

    /* one‑time initialisation of the base values */
    if (!initialized) {
        ensure_cap(4, &cache, &computed, &cap);
        cache[0] = (seq_t)1; computed[0] = 1;
        cache[1] = (seq_t)2; computed[1] = 1;
        cache[2] = (seq_t)3; computed[2] = 1;
        cache[3] = (seq_t)8; computed[3] = 1;
        cache[4] = (seq_t)11;computed[4] = 1;
        last_computed = 4;
        initialized = true;
    }

    if (n <= last_computed)            /* already known */
        return cache[n];

    ensure_cap(n, &cache, &computed, &cap);

    /* compute all missing terms up to n, iteratively using the recurrence */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        /* coefficients – work with 64‑bit signed values, then convert
           to seq_t where needed */
        int64_t i64 = (int64_t)i;
        int64_t c0 = -(i64 + 4) * (137 * i64 - 1035);
        int64_t c1 = 383 * i64 * i64 - 1622 * i64 - 5421;
        int64_t c2 = 4 * (274 * i64 * i64 - 1385 * i64 - 3534);
        int64_t c3 = 4 * (-766 * i64 * i64 + 4393 * i64 + 555);
        int64_t c4 = 16 * (-137 * i64 * i64 + 898 * i64 - 606);
        int64_t c5 = 16 * (i64 - 3) * (383 * i64 - 1622);

        seq_t pos = 0, neg = 0;

        /* a(i-1) */
        if (c1 >= 0)   pos += (seq_t)c1 * cache[i-1];
        else           neg += (seq_t)(-c1) * cache[i-1];
        /* a(i-2) */
        if (c2 >= 0)   pos += (seq_t)c2 * cache[i-2];
        else           neg += (seq_t)(-c2) * cache[i-2];
        /* a(i-3) */
        if (c3 >= 0)   pos += (seq_t)c3 * cache[i-3];
        else           neg += (seq_t)(-c3) * cache[i-3];
        /* a(i-4) */
        if (c4 >= 0)   pos += (seq_t)c4 * cache[i-4];
        else           neg += (seq_t)(-c4) * cache[i-4];
        /* a(i-5) */
        if (c5 >= 0)   pos += (seq_t)c5 * cache[i-5];
        else           neg += (seq_t)(-c5) * cache[i-5];

        seq_t res;
        if (c0 > 0) {                     /* denom positive */
            res = (neg >= pos) ? (neg - pos) / (seq_t)c0 : 0;
        } else {                           /* denom negative */
            seq_t denom = (seq_t)(-c0);
            res = (pos >= neg) ? (pos - neg) / denom : 0;
        }

        cache[i] = res;
        computed[i] = 1;
    }

    last_computed = (size_t)n;
    return cache[n];
}
