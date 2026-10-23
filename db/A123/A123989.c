/*
 * A123989  a(n) = H(2*n)*(2*n)!/n! where H(n) = Sum_{k=1..n} 1/k, the n-th
 *          harmonic number.
 * https://oeis.org/A123989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = H(2*n)*(2*n)!/n!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Return the n-th term of sequence A123989. */
seq_t a(idx_t n)
{
    /* Cached values for a and the factorial ratio F(n) = (2n)!/n! */
    static seq_t *A   = NULL;  /* a(n) cache */
    static seq_t *F   = NULL;  /* F(n) cache */
    static size_t cap = 0;     /* allocated capacity of the arrays */
    static size_t sz  = 0;     /* number of values already stored (next free index) */

    /* Initialise for n = 0. */
    if (A == NULL) {
        cap = 1;
        A = (seq_t *)malloc(cap * sizeof(seq_t));
        F = (seq_t *)malloc(cap * sizeof(seq_t));
        if (A == NULL || F == NULL) return 0;          /* OOM guard */
        A[0] = (seq_t)0;   /* a(0) = 0 */
        F[0] = (seq_t)1;   /* F(0) = 1 – convenient for the recurrence */
        sz  = 1;
    }

    /* Grow the cache if necessary. */
    if ((idx_t)n >= (idx_t)sz) {
        size_t target = (size_t)n + 1;
        if (target > cap) {
            while (cap < target) cap <<= 1;          /* double until sufficient */
            seq_t *newA = (seq_t *)realloc(A, cap * sizeof(seq_t));
            seq_t *newF = (seq_t *)realloc(F, cap * sizeof(seq_t));
            if (newA == NULL || newF == NULL) return 0;  /* OOM guard */
            A = newA;
            F = newF;
        }

        /* Compute values from the current last index up to n. */
        for (idx_t i = sz; i <= n; ++i) {
            if (i == 0) continue;    /* already initialised */

            seq_t ii = (seq_t)i;

            /* Multiplier: (2i-1)*(2i)/i = 4i-2 */
            seq_t mult = (seq_t)(4 * ii - 2);

            /* F(i) = F(i-1) * mult */
            seq_t Fi = F[i - 1] * mult;

            /* term2 = (4i-1) * (F(i-1) / i) */
            seq_t term2 = (seq_t)(4 * ii - 1) * (F[i - 1] / ii);

            /* a(i) = a(i-1) * mult + term2 */
            seq_t ai = A[i - 1] * mult + term2;

            A[i] = ai;
            F[i] = Fi;
        }

        sz = (size_t)n + 1;
    }

    return A[n];
}
