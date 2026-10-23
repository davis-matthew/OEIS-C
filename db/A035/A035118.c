/*
 * A035118  Fourier coefficients of (normalized Delta)^3.
 * https://oeis.org/A035118
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of q^(n-3) in the expansion of * q^3 *
 *            Prod_{m >= 1} (1 - q^m)^72 * * Recurrence for the coefficients
 *            a_Q of Q(q) = Prod_{m >= 1} (1 - q^m)^72: * a_Q(0) = 1 * a_Q(n)
 *            = -(k / n) * Sum_{d = 1..n} sigma_1(d) * a_Q(n - d) * where k =
 *            72 and sigma_1(d) = sum of divisors of d. * The...
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 9, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t max_n = 0;          /* maximum index requested so far */
static seq_t *sigma = NULL;      /* divisor sum sigma_1 */
static seq_t *a_Q   = NULL;      /* coefficients of Q(q) */
static size_t cap = 0;           /* allocated size of arrays */

/* Ensure sigma and a_Q are computed up to n-3 */
static void recompute_upto(idx_t n)
{
    if (n < 3) return;          /* nothing to compute for indices < 3 */

    idx_t target = n - 3;        /* we need a_Q up to this index */

    if (target + 1 <= cap) return;   /* already computed */

    /* Allocate new arrays */
    size_t new_cap = (size_t)(target + 1);
    seq_t *new_sigma = (seq_t*)malloc(new_cap * sizeof(seq_t));
    seq_t *new_aQ    = (seq_t*)malloc(new_cap * sizeof(seq_t));

    /* Zero them */
    for (size_t i = 0; i < new_cap; ++i) {
        new_sigma[i] = 0;
        new_aQ[i]    = 0;
    }

    /* Compute sigma_1 using a simple divisor sieve */
    for (idx_t i = 1; i <= target; ++i) {
        seq_t si = (seq_t)i;
        for (idx_t j = i; j <= target; j += i) {
            new_sigma[j] += si;
        }
    }

    /* Recurrence for a_Q */
    new_aQ[0] = 1;                 /* Q(0) = 1 */
    const seq_t k = 72;           /* exponent in the product */

    for (idx_t n1 = 1; n1 <= target; ++n1) {
        seq_t sum = 0;
        for (idx_t d = 1; d <= n1; ++d) {
            sum += new_sigma[d] * new_aQ[n1 - d];
        }
        new_aQ[n1] = -(k * sum) / (seq_t)n1;
    }

    /* Replace old arrays */
    free(sigma);
    free(a_Q);
    sigma = new_sigma;
    a_Q   = new_aQ;
    cap   = new_cap;
    max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* sequence is offset 3 */
    recompute_upto(n);
    return a_Q[n - 3];
}
