/*
 * A215000  a(n) = floor(exp(1 + 1/2 + 1/3 + ... + 1/n)).
 * https://oeis.org/A215000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(exp(1 + 1/2 + 1/3 + … + 1/n))
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a cached double harmonic sum. */
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 – behaviour below 1 is irrelevant */

    static double *harm = NULL;          /* harmonics H[i] = 1 + 1/2 + … + 1/i   */
    static idx_t hi = 0;                 /* highest index calculated so far      */

    /* Extend the cache if necessary */
    if (n > hi) {
        double *newharm = (double *)realloc(harm, (n + 1) * sizeof(double));
        if (!newharm) {                 /* allocation failed – unlikely for this use */
            return 0;
        }
        harm = newharm;

        /* Ensure harm[0] is 0 */
        harm[0] = 0.0;

        /* Compute harmonic sums from hi+1 up to n */
        for (idx_t i = hi + 1; i <= n; ++i) {
            harm[i] = harm[i - 1] + 1.0 / (double)i;
        }
        hi = n;
    }

    /* Evaluate exp(H[n]) and take its floor */
    double h   = harm[n];
    double ev  = exp(h);
    long long v = (long long)floor(ev + 1e-12);   /* tiny epsilon protects against FP loss */

    return (seq_t)v;
}
