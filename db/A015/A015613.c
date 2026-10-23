/*
 * A015613  a(n) = Sum_{i=1..n} phi(i) * (ceiling(n/i) - floor(n/i)).
 * https://oeis.org/A015613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002088(n) - n = sum_{i=1..n} phi(i) - n where phi(i) is
 *            Euler's totient function
 * Range:     u8 n <= 30, u16 n <= 465, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for n >= 1.  The result is stored in `cumphi`,
   a running cumulative sum of phi values.  The array is
   recomputed only when a larger n is requested. */
seq_t a(idx_t n)
{
    /* Static state to cache computed cumulative sums. */
    static seq_t *cumphi = NULL;  /* cumphi[i] = sum_{k=1..i} phi(k) */
    static idx_t maxN = 0;        /* maximum n computed so far */

    /* n is assumed >= 1 by the problem statement.  */
    if (n < 1)
        return (seq_t)0;

    /* Recompute the cache if we need values beyond maxN. */
    if (n > maxN) {
        idx_t i, j;

        /* Resize cumulative array. */
        seq_t *new_cum = (seq_t*)malloc(sizeof(seq_t) * (n + 1));
        if (!new_cum) {            /* allocation failed */
            if (cumphi) free(cumphi);
            return (seq_t)0;
        }
        if (cumphi) free(cumphi);
        cumphi = new_cum;

        /* Allocate temporary phi array. */
        seq_t *phi = (seq_t*)malloc(sizeof(seq_t) * (n + 1));
        if (!phi) {
            free(cumphi);
            cumphi = NULL;
            return (seq_t)0;
        }

        /* Initialise phi[i] = i. */
        for (i = 0; i <= n; ++i)
            phi[i] = (seq_t)i;

        /* Classic sieve for Euler's totient. */
        for (j = 2; j <= n; ++j) {
            if (phi[j] == (seq_t)j) {          /* j is prime */
                for (i = j; i <= n; i += j)
                    phi[i] -= phi[i] / (seq_t)j;
            }
        }

        /* Build cumulative sum array. */
        cumphi[0] = (seq_t)0;
        for (i = 1; i <= n; ++i)
            cumphi[i] = cumphi[i - 1] + phi[i];

        /* Clean up temporary array. */
        free(phi);

        /* Update cached maximum. */
        maxN = n;
    }

    /* a(n) = sum_{i=1..n} phi(i) - n = cumphi[n] - n */
    return cumphi[n] - (seq_t)n;
}
