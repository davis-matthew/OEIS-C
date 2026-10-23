/*
 * A144500  Column 4 of array in A144502.
 * https://oeis.org/A144500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((24*n^3 + 60*n^2 + 50*n + 5)*a(n‑1) + (12*n^2 + 24*n +
 *            1)*a(n‑2))/(12*n^2 – 11), with a(0)=1, a(1)=65
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoisation. It works for any seq_t
   that supports the +, -, *, / operators and can represent
   integers of arbitrary size (the arbitrary‑precision build) or
   unsigned integers up to 128 bits. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 0‑based array of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size of cache        */
    static size_t comp  = 0;   /* number of terms already computed */

    /* trivial values are returned immediately */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)65;

    /* ensure cache is big enough */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = needed;
        if (new_cap < 2) new_cap = 2;          /* need room for a(0) and a(1) */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                     /* out of memory */
        cache = tmp;
        cap   = new_cap;

        /* initialise the two base terms on first allocation */
        if (comp == 0) {
            cache[0] = (seq_t)1;
            if (cap > 1) cache[1] = (seq_t)65;
            comp = 2;
        }
    }

    /* compute missing terms up to n */
    if (comp <= (size_t)n) {
        for (size_t i = comp; i <= (size_t)n; ++i) {
            /* use unsigned __int128 for intermediate arithmetic
               (n and the coefficients fit easily in 128 bits) */
            __uint128_t nn  = (unsigned long long)i;
            __uint128_t n2  = nn * nn;
            __uint128_t n3  = n2 * nn;

            __uint128_t P = 24 * n3 + 60 * n2 + 50 * nn + 5;
            __uint128_t Q = 12 * n2 + 24 * nn + 1;
            __uint128_t R = 12 * n2 - 11;

            if (sizeof(seq_t) <= 8) {
                /* 64‑bit (or smaller) build – use 128‑bit temporaries */
                __uint128_t t1 = (__uint128_t)cache[i-1] * P;
                __uint128_t t2 = (__uint128_t)cache[i-2] * Q;
                cache[i] = (seq_t)((t1 + t2) / R);
            } else {
                /* arbitrary‑precision build – use seq_t operations */
                seq_t prod1 = cache[i-1] * (seq_t)P;
                seq_t prod2 = cache[i-2] * (seq_t)Q;
                seq_t num   = prod1 + prod2;
                cache[i]    = num / (seq_t)R;   /* exact division */
            }
        }
        comp = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
