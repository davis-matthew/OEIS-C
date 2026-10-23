/*
 * A188684  Partial sums of binomials binomial(3n,n)^2/(2n+1)^2.
 * https://oeis.org/A188684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{n} ( binom(3k,k)/(2k+1) )² • a(0) = 1 • for k >
 *            0, let C(k) = binom(3k,k) (known recurrence: C(k) =
 *            C(k-1)*(3k-3+3)*(3k-3+2)*(3k-3+1) / (k*(2k-2+2)*(2k-2+1) ) )
 *            D(k) = C(k)/(2k+1) (integer) term(k) = D(k)² • a(n) = a(n‑1) +
 *            term(n) The routine memoises computed a-values to answer...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed a-values */
    static seq_t *cache = NULL;        /* pointer to stored a(i) */
    static idx_t cached_len = 0;       /* number of elements stored */
    static idx_t last_k = 0;           /* last index computed */
    static seq_t last_C = (seq_t)1;    /* C(last_k) = binom(3*last_k, last_k) */

    /* initialise first term a(0) */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;          /* term for k=0 is 1 */
        cached_len = 1;
        last_k = 0;
        last_C = (seq_t)1;
    }

    /* if requested index already cached, return it */
    if (n < cached_len)
        return cache[n];

    /* extend cache up to n */
    for (idx_t k = last_k + 1; k <= n; ++k) {
        /* previous index = k-1 */
        idx_t prev = k - 1;

        /* compute C(k) from C(prev) using recurrence */
        seq_t newC = last_C;
        newC *= (seq_t)(3 * prev + 3);
        newC *= (seq_t)(3 * prev + 2);
        newC *= (seq_t)(3 * prev + 1);
        seq_t denom = (seq_t)(prev + 1) *
                      (seq_t)(2 * prev + 2) *
                      (seq_t)(2 * prev + 1);
        newC /= denom;                /* integer division, exact */

        last_C = newC;                 /* update to C(k) */

        /* compute D(k) = C(k) / (2k+1) */
        seq_t D = last_C / (seq_t)(2 * k + 1);

        /* term = D² */
        seq_t term = D * D;

        /* a(k) = a(prev) + term */
        seq_t newSum = cache[prev] + term;

        /* resize cache if necessary */
        if (cached_len <= k) {
            cache = (seq_t*)realloc(cache, sizeof(seq_t) * (k + 1));
            cached_len = k + 1;
        }
        cache[k] = newSum;
    }

    last_k = n;
    return cache[n];
}
