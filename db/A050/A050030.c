/*
 * A050030  a(n) = a(n-1) + a(m) for n >= 3, where m = n - 1 - 2^p and p is
 *          the unique integer such that 2^p < n - 1 <= 2^(p+1), starting with
 *          a(1) = a(2) = 1.
 * https://oeis.org/A050030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, a(n)=a(n-1)+a(n-1-2^p) for n≥3, where p is the
 *            unique integer with 2^p < n-1 ≤ 2^(p+1)
 * Range:     u8 n <= 41, u16 n <= 207, u32 n <= 1797, u64 n <= 8193,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoizes results and works for any order of calls. */

#include <stddef.h>
#include <stdlib.h>

/* Compute floor(log₂(v)) for v ≥ 1.  For v = 0 the behaviour is undefined. */
static inline unsigned floor_log2_idx(idx_t v)
{
    unsigned p = 0;
    /* Shift until v becomes zero, counting shifts performed. */
    while (v >>= 1) {
        ++p;
    }
    return p;
}

seq_t a(idx_t n)
{
    /* Cache of computed terms; cache[0] is unused to keep indices 1‑based. */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* highest index allocated so far */

    if (n == 0)                /* undefined for n < 1, return 0 for safety. */
        return (seq_t)0;

    if ((idx_t)cap < n) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) {            /* allocation failure – terminate the program. */
            exit(EXIT_FAILURE);
        }
        cache = tmp;

        /* Compute missing terms up to new_cap. */
        for (size_t i = cap + 1; i <= new_cap; ++i) {
            if (i == 1 || i == 2) {
                /* Initial values. */
                cache[i] = (seq_t)1;
            } else {
                /* n ≥ 3 case.  Compute p such that 2^p < n-1 ≤ 2^(p+1). */
                idx_t im1 = (idx_t)i - 1;                 /* n-1 */
                unsigned p   = floor_log2_idx(im1 - 1);   /* p = floor(log₂(n-2)) */
                idx_t pow2   = ((idx_t)1) << p;           /* 2^p */
                idx_t m      = im1 - pow2;                /* n-1-2^p (≥1) */
                cache[i] = cache[i - 1] + cache[(size_t)m];
            }
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
