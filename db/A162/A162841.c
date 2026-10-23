/*
 * A162841  G.f.: A(x) = sqrt( Sum_{n>=0} 4^(n(n+1)/2)*x^n ).
 * https://oeis.org/A162841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n) = (4^(n(n+1)/2) – Σ_{k=1}^{n-1}
 *            a(k)*a(n‑k)) / 2, i.e. the self‑convolution of a equals the
 *            series 4^(n(n+1)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* computed a(n) values */
    static idx_t  cap    = 0;       /* allocated size       */
    static idx_t  max    = 0;       /* largest n computed   */

    /* Grow the cache if needed */
    if (cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* memory allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first element once */
    if (cap > 0 && max == 0) {
        cache[0] = (seq_t)1;
    }

    /* Compute all missing terms up to n */
    if (n > max) {
        for (idx_t i = max + 1; i <= n; ++i) {
            /* exponent e = i*(i+1)/2 ; 4^e = 2^(2*e) */
            idx_t e     = i * (i + 1) / 2;
            seq_t pow4e = pow2(2 * e);      /* 2^(2e) */

            /* self‑convolution sum Σ a(k)*a(i‑k) for k=1..i-1 */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k < i; ++k) {
                sum += cache[k] * cache[i - k];
            }

            /* recurrence: 2*a(i) = pow4e – sum */
            cache[i] = (pow4e - sum) / ((seq_t)2);
        }
        max = n;
    }

    return cache[n];
}
