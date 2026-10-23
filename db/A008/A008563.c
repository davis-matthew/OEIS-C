/*
 * A008563  Coefficients of series arising in solution of Riccati equation y'
 *          = y^2 + x^2.
 * https://oeis.org/A008563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A008563 – Coefficients of series arising in solution of Riccati equation
    a(1) = 2
    a(n+1) = Σ_{k=1..n} C(4*n+2, 4*k–1) * a(k) * a(n+1–k)   for n≥1
    (C(m,r) is the binomial coefficient.  The function uses seq_t for
     arbitrary‑precision integers and idx_t for the index type.)
    The implementation memoises computed values for any order of calls.
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom.h"      /* binom(idx_t a, idx_t b) → seq_t */

seq_t a(idx_t n)
{
    /* Memoisation cache: cache[i] holds a(i) for 1 ≤ i ≤ capacity-1 */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;          /* number of allocated elements */

    if (n < 1)            /* below offset – behaviour irrelevant */
        return (seq_t)0;

    /* Expand cache if necessary */
    if (capacity < n + 1) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache)
            return (seq_t)0;               /* allocation failure */

        /* initialise newly allocated elements to 0 */
        for (idx_t i = capacity; i <= n; ++i)
            new_cache[i] = (seq_t)0;
        cache = new_cache;
        capacity = n + 1;
    }

    /* Base value a(1) */
    if (cache[1] == (seq_t)0)
        cache[1] = (seq_t)2;     /* a(1) = 2 */

    /* compute missing terms up to n */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != (seq_t)0)
            continue;            /* already known */

        seq_t sum = (seq_t)0;

        /* Parameters for the binomial coefficients */
        idx_t N = 4 * (i - 1) + 2;     /* upper index of C(N,r) */
        idx_t r = 3;                   /* first lower index 4*1-1 */

        /* current binomial value C(N,r) */
        seq_t binom_r = binom(N, r);

        for (idx_t k = 1; k < i; ++k, r += 4) {
            /* t = C(N,r) * a(k) * a(i-k) */
            seq_t t = binom_r * cache[k];
            t = t * cache[i - k];
            sum = sum + t;

            /* update binom_r to C(N, r+4) for next iteration */
            if (k < i - 1) {
                /* C(N, r+4) = C(N, r) * Π_{j=0..3} (N-(r+j))/(r+j+1) */
                for (int j = 0; j < 4; ++j) {
                    seq_t num = (seq_t)(N - (r + j));        /* N - (r+j) */
                    seq_t den = (seq_t)(r + j + 1);          /* r+j+1   */
                    binom_r = binom_r * num / den;
                }
            }
        }

        cache[i] = sum;
    }

    return cache[n];
}
