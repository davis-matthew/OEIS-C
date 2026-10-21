/*
 * A092785  a(n) = sum(sum(binomial(j-n-1,m),m=0..n),j=0..n).
 * https://oeis.org/A092785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * f(n), where f(0) = 1 and * 2*f(n)+f(n-1) =
 *            (3*n+1)*Catalan(n)+(-1)^n (n ≥ 1). * This recurrence uses the
 *            Catalan numbers C(n) = (2n)!/(n!(n+1)!)
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 16, i64 n <= 32, i128 n <= 64,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* The utils library provides the catalan(idx_t) function. */
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /*
     * f_cache[k] stores f(k) for 0 ≤ k ≤ current_last.
     * current_last holds the largest k that has been computed.
     */
    static seq_t *f_cache = NULL;
    static size_t f_cap = 0;
    static idx_t current_last = (idx_t)-1;

    /* Allocate space for the first value if needed */
    if (f_cache == NULL) {
        f_cache = (seq_t *)realloc(NULL, sizeof(seq_t));
        f_cap   = 1;
        f_cache[0] = 1;          /* f(0) = 1   */
        current_last = 0;
    }

    /* Grow the cache if required */
    if (n + 1 > f_cap) {
        size_t new_cap = (size_t)(n + 1);
        f_cache = (seq_t *)realloc(f_cache, new_cap * sizeof(seq_t));
        f_cap = new_cap;
    }

    /* Compute missing f(k) values up to n */
    for (idx_t k = current_last + 1; k <= n; ++k) {
        seq_t catal = catalan(k);                    /* Catalan(k)   */
        seq_t sign  = (k & 1) ? (seq_t)-1 : (seq_t)1; /* (-1)^k       */
        seq_t term  = (( (seq_t)3 * (seq_t)k + (seq_t)1 ) * catal) + sign;

        seq_t prev   = f_cache[k - 1];
        seq_t numerator = term - prev;                /* term - f(k-1)*/
        seq_t fk = numerator / (seq_t)2;              /* f(k) = ...   */
        f_cache[k] = fk;
    }
    current_last = n;

    /* a(n) = (-1)^n * f(n) */
    seq_t fn = f_cache[n];
    seq_t an = (n & 1) ? -fn : fn;
    return an;
}
