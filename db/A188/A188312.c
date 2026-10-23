/*
 * A188312  Expansion of (1/(1-x^2))*c(x/((1-x)*(1-x^2))) where c(x) is the
 *          g.f. of A000108.
 * https://oeis.org/A188312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A188312
 *
 *  Recurrence (5‑term linear) from the OEIS entry:
 *
 *      (n+1)*a(n) + (3-4*n)*a(n-1) + (7-6*n)*a(n-2) - a(n-3)
 *      + (n-4)*a(n-4) = 0   for n ≥ 4
 *
 *  Initial values:
 *      a(0) = 1,  a(1) = 1,  a(2) = 4,  a(3) = 12
 *
 *  The function below implements that recurrence with on‑demand memoisation.
 *  For small seq_t types (≤ 64 bits) intermediate products are
 *  computed in a __uint128_t to avoid overflow; for larger seq_t
 *  (arbitrary‑precision) the built‑in multiplication operator is used.
 *
 *  All arithmetic is done with seq_t arithmetic or with an unsigned
 *  128‑bit intermediate when necessary.  The implementation is valid C11
 *  and C++17 and obeys all requirements on types and allowed operations.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Base cases for n < 4 */
    if (n < 4) {
        static const seq_t base[4] = {
            (seq_t)1, (seq_t)1, (seq_t)4, (seq_t)12
        };
        return base[(size_t)n];
    }

    /* Static cache for computed values */
    static seq_t *cache = NULL;        /* pointer to seq_t array          */
    static size_t cap   = 0;           /* allocated size of the array    */
    static idx_t  last  = (idx_t)-1;   /* largest index already computed */

    /* Initialise cache on first use */
    if (cache == NULL) {
        cap   = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)12;
        last   = 3;
    }

    /* Grow cache if needed to accommodate index n */
    if ((size_t)n >= cap) {
        cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* Compute missing terms up to n */
    for (idx_t k = last + 1; k <= n; ++k) {
        if (sizeof(seq_t) <= 8) {
            /* Small seq_t: use 128‑bit intermediate to avoid overflow */
            __uint128_t t1 = (__uint128_t)(4 * k - 3) * (__uint128_t)cache[k - 1];
            __uint128_t t2 = (__uint128_t)(6 * k - 7) * (__uint128_t)cache[k - 2];
            __uint128_t t3 = (__uint128_t)cache[k - 3];
            __uint128_t t4 = (__uint128_t)(k - 4) * (__uint128_t)cache[k - 4];
            __uint128_t numer = t1 + t2 + t3 - t4;
            cache[k] = (seq_t)(numer / (k + 1));
        } else {
            /* Larger or arbitrary‑precision seq_t: native arithmetic is ok */
            seq_t t1 = cache[k - 1] * ((seq_t)(4 * k - 3));
            seq_t t2 = cache[k - 2] * ((seq_t)(6 * k - 7));
            seq_t t3 = cache[k - 3];
            seq_t t4 = cache[k - 4] * ((seq_t)(k - 4));
            seq_t numer = t1 + t2 + t3 - t4;
            cache[k] = numer / ((seq_t)(k + 1));
        }
    }

    last = n;
    return cache[n];
}
