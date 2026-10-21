/*
 * A063372  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          99 ).
 * https://oeis.org/A063372
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) – a(n-10) for n > 13 with initial values:
 *            a(2)= -1, a(3)=240, a(4)=521, a(5)=800, a(6)=1079, a(7)=1362,
 *            a(8)=1641, a(9)=1920, a(10)=2203, a(11)=2482, a(12)=2761,
 *            a(13)=3042
 * Range:     i8 n <= 2, i16 n <= 40, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of A063372.  n >= 2.  */
seq_t a(idx_t n)
{
    /* 16‑bit/64‑bit safe cache: values indexed by n-2 (offset 2)  */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;       /* allocated entries */
    static idx_t   max_n  = 1;       /* largest n computed (inclusive) */

    /* Undefined for indices below the offset; return 0 safely. */
    if (n < 2)
        return 0;

    /* ----------------------------------------------------------
       Allocate / initialise cache on first call.
       ---------------------------------------------------------- */
    if (cache == NULL) {
        const size_t init_len = 12;  /* 2 .. 13 inclusive */
        const seq_t init_vals[12] = {
            -1, 240, 521, 800, 1079, 1362,
            1641, 1920, 2203, 2482, 2761, 3042
        };

        cap    = 64;                /* start with more than enough space */
        if (cap < init_len) cap = init_len;

        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (cache == NULL)
            return 0;             /* allocation failed */

        for (size_t i = 0; i < init_len; ++i)
            cache[i] = init_vals[i];

        max_n = 13;                /* we now have values up to n = 13 */
    }

    /* ----------------------------------------------------------
       Return already computed value if it exists.
       ---------------------------------------------------------- */
    if (n <= max_n)
        return cache[(size_t)(n - 2)];

    /* ----------------------------------------------------------
       Grow cache if needed.
       ---------------------------------------------------------- */
    size_t needed_len = (size_t)(n - 2 + 1);   /* inclusive count of entries */
    if (cap < needed_len) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap < needed_len)
            new_cap <<= 1;          /* double until big enough */

        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (new_cache == NULL)
            return cache[(size_t)(max_n - 2)];  /* give up on growth */

        cache = new_cache;
        cap   = new_cap;
    }

    /* ----------------------------------------------------------
       Compute missing terms using the recurrence.
       ---------------------------------------------------------- */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        /* indexes in cache array = (term - 2) */
        size_t idx_k   = (size_t)(k - 2);
        size_t idx_k4  = (size_t)(k - 4 - 2);
        size_t idx_k6  = (size_t)(k - 6 - 2);
        size_t idx_k10 = (size_t)(k - 10 - 2);

        /* recurrence: a(k) = a(k-4) + a(k-6) - a(k-10) */
        cache[idx_k] = cache[idx_k4] + cache[idx_k6] - cache[idx_k10];
    }

    max_n = n;
    return cache[(size_t)(n - 2)];
}
