/*
 * A058939  The elliptic divisibility sequence associated to the rational
 *          point of smallest known global height for rational elliptic
 *          curves: the curve is [ 0,0,0,-412,3316 ] and the point is [ -18,70
 *          ].
 * https://oeis.org/A058939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   * a(2n+1) = a(n+2)*a(n)^3 - a(n-1)*a(n+1)^3 * a(2n) =
 *            a(n)*(a(n+2)*a(n-1)^2 - a(n-2)*a(n+1)^2)/a(2) * Initial terms:
 *            a(0)=0, a(1)=1, a(2)=140, * a(3)=-1372000, a(4)=-268912000000, *
 *            a(5)=1844736320000000000
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 3, i64 n <= 5, i128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Pre‑computed seed values */
static const seq_t base[6] = {
    (seq_t)0,
    (seq_t)1,
    (seq_t)140,
    (seq_t)-1372000,
    (seq_t)-268912000000,
    (seq_t)1844736320000000000
};
static const seq_t A2 = (seq_t)140;      /* a(2) */

/* Memoisation storage */
static seq_t *cache      = NULL;
static bool  *computed   = NULL;
static size_t cache_cap  = 0;

/* Recursive EDS computation */
seq_t a(idx_t n) {
    if (n <= 5) {
        return base[n];
    }

    if (n < cache_cap && computed[n]) {
        return cache[n];
    }

    seq_t res;
    if ((n & 1ULL) == 0) {                   /* n even : n = 2m */
        idx_t m      = n >> 1;
        seq_t a_m    = a(m);               /* a(m)           */
        seq_t a_m1   = a(m - 1);           /* a(m-1)         */
        seq_t a_m2   = a(m - 2);           /* a(m-2)         */
        seq_t a_m3   = a(m + 1);           /* a(m+1)         */
        seq_t a_m4   = a(m + 2);           /* a(m+2)         */

        seq_t term1   = a_m4 * (a_m1 * a_m1); /* a(m+2)*a(m-1)^2 */
        seq_t term2   = a_m2 * (a_m3 * a_m3); /* a(m-2)*a(m+1)^2 */
        seq_t numer   = term1 - term2;        /* numerator */
        res = (a_m * numer) / A2;            /* a(2m) */
    } else {                                 /* n odd : n = 2m+1 */
        idx_t m      = (n - 1) >> 1;
        seq_t a_m    = a(m);               /* a(m)           */
        seq_t a_m1   = a(m - 1);           /* a(m-1)         */
        seq_t a_m2   = a(m + 1);           /* a(m+1)         */
        seq_t a_m3   = a(m + 2);           /* a(m+2)         */

        seq_t pow_a_m   = a_m * a_m * a_m;   /* a(m)^3   */
        seq_t pow_a_m2  = a_m2 * a_m2 * a_m2;/* a(m+1)^3*/
        res = a_m3 * pow_a_m - a_m1 * pow_a_m2; /* a(2m+1) */
    }

    /* Grow cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap <= n) new_cap = n + 1;
        seq_t  *new_cache   = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        bool   *new_computed = (bool *)realloc(computed, new_cap * sizeof(bool));
        if (new_cache && new_computed) {
            for (size_t i = cache_cap; i < new_cap; ++i) {
                new_cache[i]   = 0;
                new_computed[i] = false;
            }
            cache     = new_cache;
            computed  = new_computed;
            cache_cap = new_cap;
        }
    }

    cache[n]     = res;
    computed[n]  = true;
    return res;
}
