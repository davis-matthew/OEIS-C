/*
 * A058818  a(0) = 1, a(1) = 3; for n >= 2 a(n) is the number of degree-n
 *          monic reducible polynomials over GF(3), i.e., a(n) = 3^n -
 *          A027376(n).
 * https://oeis.org/A058818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n – ⅟n · Σ_{d|n} μ(d)·3^{n/d} (for n≥2); a(0)=1, a(1)=3
 *            where μ is the Möbius function
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

static seq_t *pow3_cache = NULL;
static size_t pow3_cap = 0;

static int *mu_cache = NULL;
static size_t mu_cap = 0;

/* Ensure powers of 3 up to (n) are stored in pow3_cache */
static void ensure_pow3(idx_t n)
{
    size_t need = (size_t)n + 1;                /* indices 0 … n */
    if (need <= pow3_cap) return;

    size_t new_cap = pow3_cap ? pow3_cap : 1;
    while (new_cap < need) new_cap <<= 1;

    seq_t *new_arr = (seq_t*)realloc(pow3_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();

    if (!pow3_cache)                 /* first allocation */
    {
        new_arr[0] = (seq_t)1;        /* 3^0 */
        pow3_cap = 1;
    }
    for (size_t i = pow3_cap; i < new_cap; ++i)
        new_arr[i] = new_arr[i - 1] * (seq_t)3;

    pow3_cache = new_arr;
    pow3_cap   = new_cap;
}

/* Re‑compute the Möbius function up to (n) and store it in mu_cache */
static void ensure_mu(idx_t n)
{
    size_t need = (size_t)n + 1;                /* indices 0 … n */
    if (need <= mu_cap) return;

    size_t new_cap = mu_cap ? mu_cap : 1;
    while (new_cap < need) new_cap <<= 1;

    int *new_mu = (int*)realloc(mu_cache, new_cap * sizeof(int));
    if (!new_mu) abort();

    /* Sieve for μ up to new_cap−1 */
    int *is_comp = (int*)calloc(new_cap, sizeof(int));
    int *primes  = (int*)malloc(new_cap * sizeof(int));
    size_t pcnt  = 0;
    new_mu[1] = 1;
    for (size_t i = 2; i < new_cap; ++i)
    {
        if (!is_comp[i])      /* i is prime */
        {
            primes[pcnt++] = (int)i;
            new_mu[i] = -1;
        }
        for (size_t j = 0; j < pcnt && (size_t)primes[j]*i < new_cap; ++j)
        {
            size_t x = primes[j]*i;
            is_comp[x] = 1;
            if (i % primes[j] == 0) { new_mu[x] = 0; break; }
            else new_mu[x] = -new_mu[i];
        }
    }
    free(is_comp);
    free(primes);

    mu_cache = new_mu;
    mu_cap   = new_cap;
}

/* Sequence term a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    ensure_pow3(n);
    ensure_mu(n);

    seq_t sum = 0;
    size_t N = (size_t)n;
    for (size_t d = 1; d * d <= N; ++d)
    {
        if (N % d != 0) continue;

        int mu_d = mu_cache[d];
        if (mu_d != 0)
        {
            seq_t term = pow3_cache[N / d];
            if (mu_d > 0) sum += term;
            else          sum -= term;
        }

        size_t d2 = N / d;
        if (d2 != d)
        {
            int mu_d2 = mu_cache[d2];
            if (mu_d2 != 0)
            {
                seq_t term = pow3_cache[d];
                if (mu_d2 > 0) sum += term;
                else          sum -= term;
            }
        }
    }

    seq_t I = sum / (seq_t)n;           /* exact division */
    return pow3_cache[N] - I;
}
