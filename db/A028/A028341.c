/*
 * A028341  Coefficient of x^4 in expansion of (x+1)*(x+3)*...*(x+2*n-1).
 * https://oeis.org/A028341
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^4 in ∏_{k=1}^{n} (x + (2k-1))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a cached array of computed values.  It maintains
 * the first five coefficients of the product polynomial, updating them
 * only when a new factor (x + (2n-1)) is added.  The cache is grown
 * lazily and can serve calls in any order.  All arithmetic is performed
 * on seq_t values, using only the operators allowed by the problem
 * statement. */
 
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache for computed terms, indexed by n.  cache[0..max_n] is valid. */
    static seq_t* cache = NULL;
    static size_t cap   = 0;      /* allocated array size */
    static idx_t  max_n = 0;      /* highest n computed so far */

    /* polynomial coefficients for the first five powers of x.
     * coeff[j] holds the coefficient of x^j after processing the current
     * number of factors.  Initially only coeff[0] = 1 (empty product). */
    static seq_t coeff[5] = {1, 0, 0, 0, 0};
    static int initialized = 0;

    /* Initialise only once.  No action needed beyond setting coeff[0] to 1. */
    if (!initialized) {
        initialized = 1;
    }

    /* For n < 4 the OEIS offset specifies a zero output. */
    if (n < 4) {
        return 0;
    }

    /* If we have already computed the requested n, just return it. */
    if (n <= max_n) {
        return cache[n];
    }

    /* Expand the cache array to hold up to n. */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t* tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) { /* allocation failed */
            /* In an actual library we might handle this differently.
             * For this problem we simply abort. */
            abort();
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Extend the polynomial product from max_n + 1 up to n. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t b = (seq_t)(2 * i - 1);  /* current factor x + (2i-1) */

        /* update coefficients in decreasing order to avoid overwriting. */
        seq_t new0 = b * coeff[0];
        seq_t new1 = coeff[0] + b * coeff[1];
        seq_t new2 = coeff[1] + b * coeff[2];
        seq_t new3 = coeff[2] + b * coeff[3];
        seq_t new4 = coeff[3] + b * coeff[4];

        coeff[0] = new0;
        coeff[1] = new1;
        coeff[2] = new2;
        coeff[3] = new3;
        coeff[4] = new4;

        /* Store the value for this n.  Cache is only defined for n>=4. */
        if (i >= 4) {
            cache[i] = coeff[4];
        }
    }

    max_n = n;
    return cache[n];
}
