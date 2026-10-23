/*
 * A025271  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+ a(n-1)*a(1) for n >= 5,
 *          with initial values 1,1,2,1.
 * https://oeis.org/A025271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A025271
   a(1)=1, a(2)=1, a(3)=2, a(4)=1,
   a(n) = a(1)*a(n-1) + a(2)*a(n-2) + … + a(n-1)*a(1)   (n≥5)
   Recurrence (derived): n*a(n) = 2*(2n-3)*a(n-1) - 16*(n-6)*a(n-4)   */

/*--------------------------------------------------------------------
 *  a(n) – returns the n‑th term (n≥1).  Results are memoised in a
 *  static cache so that any order of calls is handled correctly.
 *------------------------------------------------------------------*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)1;

    /* static cache: cache[i] holds a(i+1) for i>=0 */
    static seq_t *cache = NULL;
    static idx_t cached = 0;      /* number of values already stored */
    static size_t cap = 0;        /* allocated capacity */

    /* initialise on first call */
    if (!cache) {
        cap = 8;                  /* small over‑allocation */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;     /* out of memory (unlikely) */
        cache[0] = (seq_t)1;      /* a(1) */
        cache[1] = (seq_t)1;      /* a(2) */
        cache[2] = (seq_t)2;      /* a(3) */
        cache[3] = (seq_t)1;      /* a(4) */
        cached = 4;
    }

    /* if requested value already present, return it */
    if (n <= cached) return cache[n-1];

    /* grow cache if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = n * 2;            /* safe growth factor */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n using the derived recurrence */
    for (idx_t i = cached + 1; i <= n; ++i) {
        /* coeff1 = 2*(2*i-3)  (always positive) */
        seq_t coeff1 = (seq_t)2 * (seq_t)((idx_t)2 * i - 3);

        /* term = coeff1 * a(i-1)  +/- 16*(|i-6|) * a(i-4)   */
        seq_t term = coeff1 * cache[i-2];  /* a(i-1)  is cache[i-2] */

        if (i < 6) {
            /* (n-6) is negative; add the absolute value */
            seq_t add = (seq_t)16 * (seq_t)(6 - i);   /* (6-i) positive */
            term += add * cache[i-5];                /* a(i-4) is cache[i-5] */
        } else {
            /* (n-6) is non‑negative; subtract */
            seq_t sub = (seq_t)16 * (seq_t)(i - 6);
            term -= sub * cache[i-5];
        }

        cache[i-1] = term / (seq_t)i;   /* division is exact */
    }

    cached = n;
    return cache[n-1];
}
