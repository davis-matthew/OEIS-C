/*
 * A068767  Generalized Catalan numbers 5*x*A(x)^2 -A(x) +1 -4*x=0.
 * https://oeis.org/A068767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068767 (Generalized Catalan numbers 5·A(x)^2 – A(x) + 1 – 4·x = 0)
   Recurrence:
     a(0) = 1, a(1) = 1
     a(n+1) = 5 * Σ_{k=0}^{n} a(k) * a(n-k)    for n ≥ 1 */

#include <stdlib.h>  /* malloc, realloc, free */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* grow cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;   /* out‑of‑memory: silent failure */

        /* initialise new entries */
        for (size_t idx = cache_len; idx < new_len; ++idx) {
            idx_t i = (idx_t)idx;
            if (i == 0 || i == 1) {
                tmp[idx] = (seq_t)1;
            } else {
                /* use the linear form of the recurrence for speed
                   (n+1)*a(n) = 80*(2-n)*a(n-2) + 10*(2*n-1)*a(n-1)   */
                if (sizeof(seq_t) <= 8) {  /* seq_t is 64‑bit: avoid overflow */
                    __uint128_t t1 = (__uint128_t)tmp[idx-1] * (2*i-1) * 10;
                    __uint128_t t2 = (__uint128_t)tmp[idx-2] * (i-2) * 80;
                    tmp[idx] = (seq_t)((t1 - t2) / (i + 1));
                } else {                   /* seq_t is big enough for multiplications */
                    seq_t term1 = tmp[idx-1] * (seq_t)(2*i-1) * 10;
                    seq_t term2 = tmp[idx-2] * (seq_t)(i-2) * 80;
                    tmp[idx] = (term1 - term2) / (i + 1);
                }
            }
        }
        cache = tmp;
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
