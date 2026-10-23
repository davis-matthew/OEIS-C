/*
 * A008038  Coordination sequence T1 for Zeolite Code ATS.
 * https://oeis.org/A008038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A008038: Coordination sequence T1 for Zeolite Code ATS
 *  The sequence satisfies the linear recurrence obtained from
 *  the generating function
 *
 *      G(x) = (1 +x)^2 * (1 + x + 4*x^2 + 3*x^3 + 8*x^4 + 7*x^5 + 10*x^6
 *                     +10*x^7 +9*x^8 +8*x^9 +7*x^10 +4*x^11 +x^12 +x^13 - 2*x^15)
 *              / ((1 - x)^3 * (1 - x + x^2) * (1 + x + x^2) *
 *                 (1 + x + x^2 + x^3 + x^4)^2).
 *
 *  If D(x)=∑_{j=0}^{15} d_j x^j and N(x)=∑_{j=0}^{17} n_j x^j
 *  then a(n) satisfies (for all n≥0)
 *
 *      a(n) = n_n   –  ∑_{j=1}^{min(15,n)} d_j · a(n−j),
 *
 *  where n_n = n_j for n≤17 and 0 otherwise.
 *  The coefficients are:
 *      d = [ 1, -1,  1, -1,  1, -3,  2, -2,  2, -2,  3, -1,  1, -1,  1, -1 ]
 *      n = [ 1,  3,  7, 12, 18, 26, 32, 37, 39, 36, 32, 26, 16, 7, 3, -1,-4,-2 ]
 *
 *  The implementation keeps a static cache of all needed terms and
 *  extends it only when larger indices are requested.
 */

#include <stdlib.h>

/* coefficients of the denominator polynomial (order 15) */
static const int D[16] = { 1, -1,  1, -1,  1, -3,  2, -2,  2, -2,  3, -1,  1, -1,  1, -1 };

/* coefficients of the numerator polynomial (order 17) */
static const int N[18] = { 1,  3,  7, 12, 18, 26, 32, 37, 39, 36, 32, 26, 16,  7,  3, -1, -4, -2 };

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;      /* how many terms are stored */
    static idx_t computed   = (idx_t)-1; /* last index actually computed */

    /* enlarge the cache if necessary */
    if (cache_cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (!new_cache) abort();              /* allocation failure */

        /* keep already computed values */
        if (cache) {
            for (idx_t i = 0; i <= computed; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* compute all terms up to n that are not yet known */
    idx_t start = (computed == (idx_t)-1) ? 0 : computed + 1;
    for (idx_t k = start; k <= n; ++k) {
        int64_t val = 0;

        /* contribution of the numerator term */
        if (k < (idx_t)(sizeof(N)/sizeof(N[0])))
            val += (int64_t)N[k];

        /* contributions from the recurrence defined by D */
        for (int j = 1; j <= 15 && j <= k; ++j) {
            int coeff = D[j];
            if (coeff > 0)
                val -= (int64_t)cache[k - j] * coeff;
            else if (coeff < 0)
                val += (int64_t)cache[k - j] * (-coeff);
        }

        cache[k] = (seq_t)val;   /* val is non‑negative for all k */
    }

    computed = n;
    return cache[n];
}
