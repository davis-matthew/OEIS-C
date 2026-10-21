/*
 * A054137  T(n,3), array T as in A054134.
 * https://oeis.org/A054137
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(3) = 2, a(4) = 8, a(5) = 31. For n ≥ 6, d(4) = 4, d(5) = 15,
 *            d(n) = d(n‑1) + C(n,3), a(n) = 2·a(n‑1) + d(n). (C(n,3) =
 *            n·(n‑1)·(n‑2)/6.)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *a_cache = NULL;   /* stores a values */
    static seq_t *d_cache = NULL;   /* stores d values (a(n)-2*a(n-1)) */
    static idx_t cap   = 0;         /* allocated capacity of the arrays */
    static idx_t last_n = 0;         /* highest n computed */

    /* ------------------------------------------------------------ */
    /* initialise cache for the first call                         */
    if (a_cache == NULL) {
        cap = 7; /* indices 0..6 will suffice initially */
        a_cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        d_cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        for (idx_t i = 0; i < cap; ++i) {
            a_cache[i] = 0;
            d_cache[i] = 0;
        }
        /* base values */
        a_cache[3] = 2;
        a_cache[4] = 8;
        a_cache[5] = 31;
        d_cache[4] = 4;  /* 8 - 2*2  */
        d_cache[5] = 15; /* 31 - 2*8 */
        last_n = 5;
    }

    /* ------------------------------------------------------------ */
    /* handle trivial or already computed requests                 */
    if (n <= last_n) return a_cache[n];
    if (n < 3) return 0;   /* undefined, but harmless */

    /* ------------------------------------------------------------ */
    /* grow arrays if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_cap);
        d_cache = (seq_t *)realloc(d_cache, sizeof(seq_t) * new_cap);
        /* initialise the newly allocated part to zero */
        for (idx_t i = cap; i < new_cap; ++i) {
            a_cache[i] = 0;
            d_cache[i] = 0;
        }
        cap = new_cap;
    }

    /* ------------------------------------------------------------ */
    /* compute missing values up to n */
    for (idx_t i = last_n + 1; i <= n; ++i) {
        seq_t bin = 0;
        if (i >= 6) {
            unsigned long long t = (unsigned long long)i;
            bin = (seq_t)(t * (t - 1) * (t - 2) / 6);
        }
        /* d(i) = d(i-1) + C(i,3)   (valid for i ≥ 6) */
        d_cache[i] = d_cache[i - 1] + bin;
        /* a(i) = 2*a(i-1) + d(i) */
        a_cache[i] = a_cache[i - 1] * 2 + d_cache[i];
    }

    last_n = n;
    return a_cache[n];
}
