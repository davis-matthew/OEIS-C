/*
 * A009405  Expansion of e.g.f. log(1+x)*cos(log(1+x)).
 * https://oeis.org/A009405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   (n^4 + 2*n^2 + 1)*a(n) + (4*n^3 + 6*n^2 + 8*n + 3)*a(n + 1) +
 *            3*(2*n^2 + 4*n + 3)*a(n + 2) + 2*(2*n + 3)*a(n + 3) + a(n + 4) =
 *            0, with initial values a(0)=0, a(1)=1, a(2)=-1, a(3)=-1
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache   = NULL;   /* pointer to cache array */
    static idx_t  size    = 0;      /* number of terms stored (cache[0..size-1]) */
    static idx_t  cap     = 0;      /* allocated capacity in the cache */

    /* initialise cache with first four terms on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 4);
        if (cache == NULL) exit(EXIT_FAILURE);  /* allocation failure */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = -1;
        cache[3] = -1;
        size = 4;
        cap  = 4;
    }

    /* return directly if already computed */
    if (n < size)
        return cache[n];

    /* grow cache step by step using the recurrence */
    for (idx_t i = size; i <= n; ++i) {
        idx_t idx = i - 4;          /* recurrence index n corresponds to i-4 */

        /* compute coefficients using seq_t arithmetic to avoid overflow */
        seq_t nseq   = (seq_t)idx;
        seq_t coeff0 = nseq*nseq*nseq*nseq + 2*nseq*nseq + 1;
        seq_t coeff1 = 4*nseq*nseq*nseq + 6*nseq*nseq + 8*nseq + 3;
        seq_t coeff2 = 3 * ( 2*nseq*nseq + 4*nseq + 3 );
        seq_t coeff3 = 2 * ( 2*nseq + 3 );

        /* compute a(i) via the recurrence */
        seq_t sum = coeff0*cache[idx]     + 
                    coeff1*cache[idx+1]   + 
                    coeff2*cache[idx+2]   + 
                    coeff3*cache[idx+3];
        seq_t val = -sum;  /* a(i) = -(sum) */

        /* ensure sufficient capacity */
        if (size >= cap) {
            size_t newcap = cap * 2;
            if (newcap == 0) newcap = 4;        /* should not happen, but guard */
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
            if (tmp == NULL) exit(EXIT_FAILURE);
            cache = tmp;
            cap   = newcap;
        }

        /* store and advance */
        cache[i] = val;
        ++size;
    }

    return cache[n];
}
