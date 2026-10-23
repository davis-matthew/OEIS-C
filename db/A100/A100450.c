/*
 * A100450  Number of ordered triples (i,j,k) with |i| + |j| + |k| <= n and
 *          gcd(i,j,k) <= 1.
 * https://oeis.org/A100450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   comb3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 + Σ_{d=1..n} μ(d) * (F(⌊n/d⌋) – 1)
 * where F(m) = Σ_{j=0..min(3,m)} C(3,j) · C(m−j+3,3)
 * and C(n,k) is the binomial coefficient.
 * (The term 1 accounts for the triple (0,0,0).)
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/comb3.h"

/* static tables for μ */
static int8_t  *mu_tab  = NULL;   /* μ[0..]   */
static uint8_t *is_comp = NULL;   /* composite flag for sieve */
static idx_t  mu_size = 0;

/* compute μ[0..n] using the linear sieve */
static void compute_mu_upto(idx_t n)
{
    if (mu_size > n) return; /* already computed */

    idx_t new_size = n + 1;
    mu_tab  = (int8_t *)realloc(mu_tab,  new_size * sizeof(int8_t));
    is_comp = (uint8_t*)realloc(is_comp, new_size * sizeof(uint8_t));

    /* initialise from previous size */
    for (idx_t i = mu_size; i <= n; ++i) {
        mu_tab[i]  = 0;      /* 0 indicates not yet processed   */
        is_comp[i] = 0;      /* 0 = prime, 1 = composite         */
    }
    mu_tab[0] = 0;
    mu_tab[1] = 1;
    mu_size = new_size;

    idx_t *primes = (idx_t*)malloc(new_size * sizeof(idx_t));
    idx_t pc = 0;

    for (idx_t i = 2; i <= n; ++i) {
        if (is_comp[i] == 0) {          /* i is prime */
            primes[pc++] = i;
            mu_tab[i] = -1;
        }
        for (idx_t j = 0; j < pc; ++j) {
            idx_t p = primes[j];
            idx_t v = i * p;
            if (v > n) break;
            is_comp[v] = 1;
            if (i % p == 0) {
                mu_tab[v] = 0;
                break;
            } else {
                mu_tab[v] = -mu_tab[i];
            }
        }
    }
    free(primes);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 1;  /* only the triple (0,0,0) */

    compute_mu_upto(n);

    /* coefficients C(3,j) */
    static const int C3[4] = {1, 3, 3, 1};

    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        int8_t mu = mu_tab[d];
        if (mu == 0) continue;
        idx_t t = n / d;           /* floor(n/d) */
        /* compute F(t) */
        seq_t Ft = 0;
        idx_t maxj = t < 3 ? t : 3;    /* min(3,t) */
        for (idx_t j = 0; j <= maxj; ++j) {
            seq_t term = (seq_t)C3[j] * comb3(t - j + 3);
            Ft += term;
        }
        seq_t contrib = Ft - 1;     /* subtract the (0,0,0) term */
        if (mu == 1)
            sum += contrib;
        else
            sum -= contrib; /* mu == -1 */
    }
    return sum + 1;   /* add the (0,0,0) triple back */
}
