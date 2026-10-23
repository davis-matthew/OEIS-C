/*
 * A175254  a(n) = Sum_{k<=n} A000203(k)*(n-k+1), where A000203(m) is the sum
 *          of divisors of m.
 * https://oeis.org/A175254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k<=n} A000203(k)*(n-k+1)
 * Range:     u8 n <= 9, u16 n <= 61, u32 n <= 2501, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t max_n = 0;
static seq_t *sigma = NULL;
static seq_t *prefix_sigma = NULL;
static seq_t *prefix_ksigma = NULL;

/* Recompute sigma and prefix sums up to new_max */
static void recompute(idx_t new_max) {
    if (new_max == max_n) return;

    /* Allocate new arrays */
    seq_t *new_sigma = (seq_t *)calloc(new_max + 1, sizeof(seq_t));
    seq_t *new_prefix_sigma = (seq_t *)calloc(new_max + 1, sizeof(seq_t));
    seq_t *new_prefix_ksigma = (seq_t *)calloc(new_max + 1, sizeof(seq_t));

    /* Compute sigma using a simple sieve */
    for (idx_t d = 1; d <= new_max; ++d) {
        seq_t sd = (seq_t)d;
        for (idx_t j = d; j <= new_max; j += d)
            new_sigma[j] += sd;
    }

    /* Compute prefix sums of sigma and of k*sigma(k) */
    new_prefix_sigma[0] = 0;
    new_prefix_ksigma[0] = 0;
    for (idx_t k = 1; k <= new_max; ++k) {
        seq_t s = new_sigma[k];
        new_prefix_sigma[k] = new_prefix_sigma[k - 1] + s;
        new_prefix_ksigma[k] = new_prefix_ksigma[k - 1] + (seq_t)k * s;
    }

    /* Free old arrays */
    free(sigma);
    free(prefix_sigma);
    free(prefix_ksigma);

    /* Store new arrays */
    sigma = new_sigma;
    prefix_sigma = new_prefix_sigma;
    prefix_ksigma = new_prefix_ksigma;
    max_n = new_max;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    if (n > max_n)
        recompute(n);

    seq_t sum_sigma   = prefix_sigma[n];
    seq_t sum_k_sigma = prefix_ksigma[n];

    return ((seq_t)(n + 1) * sum_sigma) - sum_k_sigma;
}
