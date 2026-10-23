/*
 * A175341  Number of coprime pairs (x,y) with x^2+y^2 <= n^2.
 * https://oeis.org/A175341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of coprime integer pairs (x,y) with x²+y² ≤ n². *
 *            The algorithm uses Möbius inversion: * a(n) = 4 * ( Σ_{d=1..n}
 *            μ(d) * C( floor(n²/d²) ) ) + 4, n > 0 * where μ is the Möbius
 *            function and * C(R) = #{ (x,y) : x,y ≥ 1, x² + y² ≤ R }. * The
 *            complete set of lattice points is obtained by...
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt works on seq_t */

static int8_t *mu = NULL;      /* Möbius array 1..n */
static idx_t mu_len = 0;       /* current allocated length */

/* Build Möbius function up to n (inclusive) using linear sieve */
static void compute_mu_upto(idx_t n)
{
    if (mu_len > 0 && mu_len-1 >= n) return;      /* already done */

    size_t sz = (size_t)n + 1;
    int8_t *new_mu = (int8_t *)realloc(mu, sz * sizeof(int8_t));
    if (!new_mu) exit(EXIT_FAILURE);
    mu = new_mu;

    /* Auxiliary arrays */
    unsigned char *is_comp = (unsigned char *)calloc(sz, 1);
    if (!is_comp) exit(EXIT_FAILURE);
    int *primes = (int *)malloc(sz * sizeof(int));
    if (!primes) exit(EXIT_FAILURE);
    size_t pcnt = 0;

    mu[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            primes[pcnt++] = (int)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            idx_t p = primes[j];
            idx_t ip = i * p;
            if (ip > n) break;
            is_comp[ip] = 1;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }
    free(is_comp);
    free(primes);
    mu_len = sz;
}

/* Count lattice points (x,y) with x,y ≥ 1 and x² + y² ≤ R (R ≥ 0) */
static seq_t count_pairs_bound(idx_t R)
{
    if (R == 0) return 0;
    idx_t x_max = (idx_t)floor_sqrt((seq_t)R);
    seq_t total = 0;
    for (idx_t x = 1; x <= x_max; ++x) {
        idx_t rem = R - x * x;                /* rem ≥ 0 */
        seq_t y_max = floor_sqrt((seq_t)rem);
        total += y_max;                       /* y ranges from 1..y_max */
    }
    return total;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    compute_mu_upto(n);

    idx_t n_sq = (idx_t)n * (idx_t)n;
    int64_t inner = 0;    /* sign‑sensitive intermediate value */

    for (idx_t d = 1; d <= n; ++d) {
        int8_t mu_d = mu[d];
        if (mu_d == 0) continue;
        idx_t d_sq = d * d;
        idx_t R = n_sq / d_sq;    /* floor of n²/d² */
        if (R == 0) continue;
        seq_t cnt = count_pairs_bound(R);
        inner += (int64_t)mu_d * (int64_t)cnt;
    }

    seq_t result = (seq_t)(inner * 4 + 4);  /* add the 4 axial points */
    return result;
}
