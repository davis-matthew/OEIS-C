/*
 * A118930  E.g.f.: A(x) = exp( Sum_{n>=0} x^(2^n)/2^(2^n-1) ).
 * https://oeis.org/A118930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} n!/(k!(n-2k)!2^k) * a(k), a(0)=1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: A(n,0)=1 ; A(n,k)=A(n,k-1)*(n-2k+2)*(n-2k+1)/(2k) */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initial allocation for a(0) */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure */
        cache[0] = 1;
        cache_len = 1;
    }

    /* Resize cache if necessary */
    if ((idx_t)cache_len <= n) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms up to n */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            idx_t I = (idx_t)i;
            idx_t maxk = I / 2;
            seq_t sum = 0;
            seq_t A = 1;                /* corresponds to A(I,0) */

            /* k = 0 term */
            if (0 <= maxk) {
                sum += cache[0] * A;   /* cache[0] == 1 */
            }

            /* Subsequent terms */
            for (idx_t k = 1; k <= maxk; ++k) {
                seq_t num1 = I - 2 * k + 2;
                seq_t num2 = I - 2 * k + 1;
                /* Update A = A * num1 * num2 / (2*k) */
                /* All operations are exact integer divisions */
                A = (A * num1 * num2) / ((seq_t)2 * k);
                sum += cache[(size_t)k] * A;
            }

            cache[i] = sum;
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
