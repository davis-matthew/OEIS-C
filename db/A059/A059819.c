/*
 * A059819  Expansion of series related to Liouville's Last Theorem: g.f.
 *          Sum_{t>0} (-1)^(t+1) *x^(t*(t+1)/2) / ( (1-x^t)^2
 *          *Product_{i=1..t} (1-x^i) ).
 * https://oeis.org/A059819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1115, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (sigma(n)+tau(n)+Sum_{k=0..n} tau(k)*tau(n-k))/2,
 * with sigma(0)=tau(0)=0.
 * sigma(n) = sum of divisors of n,
 * tau(n)   = number of divisors of n.
 *
 * The implementation precomputes divisor sums and counts using a sieve
 * and recomputes incrementally when a larger n is requested.
 */
#include <stdlib.h>

static idx_t max_prepared = 0;
static seq_t *sigma_table = NULL;
static seq_t *tau_table   = NULL;

static void prepare_tables(idx_t n) {
    if (n <= max_prepared)
        return;

    seq_t *new_sigma = NULL;
    seq_t *new_tau   = NULL;

    size_t new_size = (size_t)n + 1;
    new_sigma = (seq_t*)calloc(new_size, sizeof(seq_t));
    new_tau   = (seq_t*)calloc(new_size, sizeof(seq_t));
    if (!new_sigma || !new_tau) {
        /* Allocation failed – terminate program safely */
        abort();
    }

    /* tau[0] = sigma[0] = 0 already by calloc */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            new_tau[j]   += (seq_t)1;
            new_sigma[j] += (seq_t)i;
        }
    }

    /* Replace old tables with new ones */
    free(sigma_table);
    free(tau_table);
    sigma_table = new_sigma;
    tau_table   = new_tau;
    max_prepared = n;
}

seq_t a(idx_t n) {
    if (n == (idx_t)0)
        return (seq_t)0;  /* a(0) is defined as 0 */

    prepare_tables(n);

    /* Compute convolution sum of tau with itself up to n */
    seq_t conv_sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        conv_sum += tau_table[k] * tau_table[n - k];
    }

    seq_t result = (sigma_table[n] + tau_table[n] + conv_sum) / 2;
    return result;
}
