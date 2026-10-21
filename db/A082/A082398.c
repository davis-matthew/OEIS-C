/*
 * A082398  Number of directed, diagonally convex polyominoes with n cells.
 * https://oeis.org/A082398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficients of G(x) =
 *            (x*(1-4*x+6*x^2-3*x^3+x^4))/((1-x)*(1-3*x+x^2)^2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 1..7 the values are fixed: 1,0,3,10,33,106,331 and for n ≥ 8 they satisfy */
/* a(n) = 7 a(n-1) – 17 a(n-2) + 17 a(n-3) – 7 a(n-4) + a(n-5). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* 1‑based indexing: cache[1] = a(1) */
    static idx_t cached = 0;         /* largest n that has been computed */

    if (n < 1)                        /* offset is 1; behavior below irrelevant */
        return (seq_t)0;

    /* Grow the cache if necessary */
    if (n > cached) {
        idx_t new_cap = n + 1;            /* reserve index 0 unused */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* unlikely to happen in tests */
        cache = tmp;

        /* Initialise the first seven terms (override if already present) */
        static const seq_t init[8] = {
            0,          /* dummy for index 0 */
            (seq_t)1,   /* a(1) */
            (seq_t)0,   /* a(2) */
            (seq_t)3,   /* a(3) */
            (seq_t)10,  /* a(4) */
            (seq_t)33,  /* a(5) */
            (seq_t)106, /* a(6) */
            (seq_t)331  /* a(7) */
        };
        for (idx_t i = cached + 1; i <= n && i <= 7; ++i)
            cache[i] = init[i];

        /* Compute further terms using the recurrence */
        for (idx_t i = 8; i <= n; ++i) {
            /* a(i) = 7*a(i-1) – 17*a(i-2) + 17*a(i-3) – 7*a(i-4) + a(i-5) */
            seq_t t1 = 7     * cache[i-1];
            seq_t t2 = -17   * cache[i-2];
            seq_t t3 = 17    * cache[i-3];
            seq_t t4 = -7    * cache[i-4];
            seq_t t5 =      cache[i-5];
            cache[i] = t1 + t2 + t3 + t4 + t5;
        }

        cached = n;
    }
    return cache[n];
}
