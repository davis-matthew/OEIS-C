/*
 * A052603  E.g.f. (1-x)^3/(1-4x+3x^2-x^3).
 * https://oeis.org/A052603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=8, a(3)=78;
   For n ≥ 1:
   a(n+3) = (n^3+6*n^2+11*n+6)·a(n)
            - (18+3*n^2+15*n)·a(n+1)
            + (4*n+12)·a(n+2). */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  max_n = 0;          /* largest n computed so far */

    /* Initialise cache with the first four values. */
    if (!cache) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)78;
        max_n = 3;
    }

    if (n <= max_n) {                 /* value already known */
        return cache[n];
    }

    /* Resize cache so that it can hold indices 0 … n. */
    if (n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap <= n) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* memory exhaustion */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        /* m ≥ 4 → recurrence index k = m-3 ≥ 1 */
        idx_t k = m - 3;
        seq_t kn = (seq_t)k;
        seq_t n2 = kn * kn;
        seq_t n3 = n2 * kn;

        /* a(m) = termA - termB + termC */
        seq_t termA = (n3 + (seq_t)6 * n2 + (seq_t)11 * kn + (seq_t)6) * cache[k];
        seq_t termB = ((seq_t)18 + (seq_t)3 * n2 + (seq_t)15 * kn) * cache[k + 1];
        seq_t termC = ((seq_t)4 * kn + (seq_t)12) * cache[k + 2];

        cache[m] = termA - termB + termC;
    }

    max_n = n;
    return cache[n];
}
