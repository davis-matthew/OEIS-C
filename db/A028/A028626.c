/*
 * A028626  Expansion of (theta_3(z)*theta_3(15z) +
 *          theta_2(z)*theta_2(15z))^2.
 * https://oeis.org/A028626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in (theta_3(z)*theta_3(15z) +
 *            theta_2(z)*theta_2(15z))^2
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static cache for the intermediate coefficient array A */
static seq_t *A_cache = NULL;      /* array of coefficients A[n] */
static idx_t    A_max  = 0;        /* highest index for which A_cache is valid */

/* recompute A_cache up to index N (inclusive) */
static void compute_A(idx_t N)
{
    if (N < A_max) return;          /* already computed up to a larger N */

    /* Allocate new array of size N+1 and zero it */
    seq_t *newA = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!newA) return;              /* allocation failed: silently return */
    memset(newA, 0, (N + 1) * sizeof(seq_t));

    /* θ3(z)*θ3(15z) part : indices i,j >= 0 with i^2 + 15 j^2 = n */
    for (idx_t i = 0; ; ++i) {
        idx_t i2 = i * i;
        if (i2 > N) break;
        for (idx_t j = 0; ; ++j) {
            idx_t idx = i2 + 15 * j * j;
            if (idx > N) break;
            seq_t mult = ((i == 0) ? (seq_t)1 : ((seq_t)2)) *
                         ((j == 0) ? (seq_t)1 : ((seq_t)2));
            newA[idx] += mult;
        }
    }

    /* θ2(z)*θ2(15z) part : indices i,j >= 0 with 4 + i(i+1) + 15 j(j+1) = n */
    for (idx_t i = 0; ; ++i) {
        idx_t i_term = i * (i + 1);
        if (4 + i_term > N) break;
        for (idx_t j = 0; ; ++j) {
            idx_t idx = 4 + i_term + 15 * j * (j + 1);
            if (idx > N) break;
            newA[idx] += (seq_t)4;
        }
    }

    /* Replace old array */
    free(A_cache);
    A_cache = newA;
    A_max   = N;
}

/* public function */
seq_t a(idx_t n)
{
    /* ensure coefficients A up to n are available */
    compute_A(n);

    /* sum_{k=0}^{n} A[k] * A[n-k] */
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        sum += A_cache[k] * A_cache[n - k];
    }
    return sum;
}
