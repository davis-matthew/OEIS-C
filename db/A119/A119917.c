/*
 * A119917  Number of rationals in [0, 1) consisting just of repeating bits of
 *          period at most n.
 * https://oeis.org/A119917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{i=1..n} sum_{d|i} (2^d - 1) * mu(i/d)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes the Möbius function and the powers of 2
   up to the largest argument requested.  For each newly required n it
   recomputes the Dirichlet convolution
        f[i] = sum_{d|i} (2^d-1) * mu(i/d)
   and builds the prefix sums a(k)=∑_{i=1..k}f[i].
   The function is memoised across calls. */

#include <stdlib.h>   /* malloc, realloc, calloc, free */
#include <stdint.h>   /* uint64_t */

/* Forward declaration of the required function signature */
seq_t a(idx_t n);

/* Compute Möbius μ[1..limit] with a linear sieve.
   The function guarantees that all entries 1..limit are filled. */
static void compute_mu(idx_t limit, int *mu)
{
    if (limit == 0)
        return;
    mu[1] = 1;

    int max = (int)limit;
    int *spf   = (int *)calloc(max + 1, sizeof(int));
    int *is_comp = (int *)calloc(max + 1, sizeof(int));
    int *primes = (int *)calloc(max / 2 + 2, sizeof(int));
    int primes_cnt = 0;

    for (int i = 2; i <= max; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            spf[i] = i;
            mu[i] = -1;
        }
        for (int j = 0; j < primes_cnt && (long long)i * primes[j] <= max; ++j) {
            int p = primes[j];
            int x = i * p;
            is_comp[x] = 1;
            spf[x] = p;
            if (i % p == 0) {
                mu[x] = 0;
                break;
            } else {
                mu[x] = -mu[i];
            }
        }
    }

    free(spf);
    free(is_comp);
    free(primes);
}

/* Main function */
seq_t a(idx_t n)
{
    /* Memoised state */
    static idx_t max_computed = 0;
    static seq_t *prefix = NULL;   /* prefix[k] = a(k) */
    static seq_t *pow2 = NULL;     /* pow2[d] = 2^d */
    static int *mu = NULL;         /* Möbius values */

    if (n == 0) return (seq_t)0; /* safety for calls with n < 1 */

    if (n > max_computed) {
        idx_t new_lim = n;
        size_t new_size = (size_t)(new_lim + 1);

        /* Reallocate prefix array */
        if (prefix) {
            prefix = (seq_t *)realloc(prefix, new_size * sizeof(seq_t));
        } else {
            prefix = (seq_t *)malloc(new_size * sizeof(seq_t));
        }

        /* Reallocate pow2 array */
        if (pow2) {
            pow2 = (seq_t *)realloc(pow2, new_size * sizeof(seq_t));
        } else {
            pow2 = (seq_t *)malloc(new_size * sizeof(seq_t));
        }

        /* Reallocate mu array */
        if (mu) {
            mu = (int *)realloc(mu, new_size * sizeof(int));
        } else {
            mu = (int *)malloc(new_size * sizeof(int));
        }

        /* Compute powers of 2 up to new_lim */
        pow2[0] = (seq_t)1;
        for (idx_t d = 1; d <= new_lim; ++d) {
            pow2[d] = pow2[d - 1] * (seq_t)2;
        }

        /* Compute Möbius function up to new_lim */
        compute_mu(new_lim, mu);

        /* Allocate temporary array for f[i] */
        seq_t *f = (seq_t *)malloc(new_size * sizeof(seq_t));
        for (idx_t i = 0; i <= new_lim; ++i) f[i] = (seq_t)0;

        /* Compute f[i] = Σ_{d|i} (2^d-1)*μ(i/d) */
        for (idx_t d = 1; d <= new_lim; ++d) {
            seq_t term = pow2[d] - (seq_t)1;
            for (idx_t m = d; m <= new_lim; m += d) {
                int mu_val = mu[m / d];
                if (mu_val == 0) continue;
                if (mu_val == 1) {
                    f[m] = f[m] + term;
                } else { /* mu_val == -1 */
                    f[m] = f[m] - term;
                }
            }
        }

        /* Build prefix sums a(k) = Σ_{i=1..k} f[i] */
        if (max_computed == 0) {
            prefix[0] = (seq_t)0;
        }
        for (idx_t i = max_computed + 1; i <= new_lim; ++i) {
            prefix[i] = prefix[i - 1] + f[i];
        }
        max_computed = new_lim;

        free(f);
    }

    return prefix[n];
}
