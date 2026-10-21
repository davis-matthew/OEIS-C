/*
 * A050147  a(n) = T(n,n-1), array T as in A050143. Also T(2n+1,n), array T as
 *          in A055807.
 * https://oeis.org/A050147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2n+3) a(n-1) + 3(8n-29) a(n-2) – (10n-49) a(n-3) +
 *            (n-6) a(n-4)) / (n-1); Initial values: a(1)=1, a(2)=3, a(3)=12,
 *            a(4)=56, a(5)=280, a(6)=1452
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache: cache[0] = a(1), cache[1] = a(2), ... */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated space */
    static idx_t last = 0;      /* number of elements stored */

    /* first numbers */
    static const seq_t init[6] = {1,3,12,56,280,1452};

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 64;                      /* arbitrary start size */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        for (idx_t i = 0; i < 6; ++i) cache[i] = init[i];
        last = 6;
    }

    if (n <= last)             /* already computed */
        return cache[n-1];

    /* extend cache up to n */
    if (n > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < n) new_cap = n;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    for (idx_t k = last + 1; k <= n; ++k) {
        if (k <= 6) {                     /* fill the known entries if needed */
            cache[k-1] = init[k-1];
            continue;
        }

        /* a(k-1), a(k-2), a(k-3), a(k-4) */
        seq_t t1 = cache[k-2];
        seq_t t2 = cache[k-3];
        seq_t t3 = cache[k-4];
        seq_t t4 = cache[k-5];

        /* coefficients */
        seq_t c1 = (seq_t)(2*k + 3);
        seq_t c2 = (seq_t)(3*(8*k - 29));
        seq_t c3 = (seq_t)(10*k - 49);   /* will be subtracted */
        seq_t c4 = (seq_t)(k - 6);

        /* numerator = c1*t1 + c2*t2 - c3*t3 + c4*t4 */
        seq_t numerator = c1 * t1;
        numerator += c2 * t2;
        numerator += c4 * t4;
        numerator -= c3 * t3;            /* subtraction of a non‑negative term */

        seq_t denominator = (seq_t)(k - 1);
        cache[k-1] = numerator / denominator;   /* exact division */
    }

    last = n;
    return cache[n-1];
}
