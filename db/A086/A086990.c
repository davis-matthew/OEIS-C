/*
 * A086990  Expansion of (1+4x-sqrt(1+4x^2))/(4+6x) in powers of x.
 * https://oeis.org/A086990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 25, i32 n <= 42, i64 n <= 75, i128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the functional equation
 * 0 = x – (4x+1)A(x) + (3x+2)A(x)^2.
 * Let a(0)=0. For n>=1:
 *   a(n) = -4*a(n-1)
 *          + 2 * sum_{k=1}^{n-1} a(k)*a(n-k)
 *          + 3 * sum_{k=1}^{n-2} a(k)*a(n-1-k)
 *          + (n==1?1:0)
 */
#include <stdlib.h>

extern seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t upto = 0;  /* next index to compute (cache[upto] not yet set) */

    /* Initialise cache if needed */
    if (!cache) {
        cap = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();                 /* out of memory */
        cache[0] = 0;                        /* a(0) */
        upto = 1;                            /* next to compute */
    }

    /* Grow cache to at least n+1 entries */
    if (n >= cap) {
        size_t new_cap = cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    while (upto <= n) {
        idx_t k = upto;                     /* compute a(k) */
        seq_t sum_c = 0;                    /* sum_{i=1}^{k-1} a(i)*a(k-i) */
        for (idx_t i = 1; i < k; ++i)
            sum_c += cache[i] * cache[k - i];

        seq_t sum_c_prev = 0;                /* sum_{i=1}^{k-2} a(i)*a(k-1-i) */
        if (k > 1)
            for (idx_t i = 1; i + 1 < k; ++i)
                sum_c_prev += cache[i] * cache[(k - 1) - i];

        seq_t rhs = (k == 1) ? 1 : 0;
        seq_t ak = -4 * cache[k - 1] + 2 * sum_c + 3 * sum_c_prev + rhs;
        cache[upto++] = ak;                  /* store computed term */
    }

    return cache[n];
}
