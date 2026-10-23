/*
 * A049697  a(n)=T(n,n+1), array T as in A049695.
 * https://oeis.org/A049697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = ∑_{k=1}^{n+1} μ(k) * ⌊(n+1)/k⌋ * ⌈(n+1)/k⌉
 * (μ is the Möbius function)
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- Möbius function sieve ---------- */
static int *mu = NULL;        /* µ[0] unused, µ[1] = 1 */
static idx_t mu_len = 0;      /* size of mu[0..mu_len-1] */

/* Ensure that µ(k) is available for all 0 ≤ k ≤ up_to.  The function
 * recomputes the whole array from scratch whenever it needs to grow.  */
static void
ensure_mu(idx_t up_to)
{
    /* Already have enough entries? */
    if (mu_len > 0 && mu_len - 1 >= up_to)
        return;

    idx_t new_len = up_to + 1;          /* need indices 0..up_to */
    int *new_mu = (int*) calloc((size_t)new_len, sizeof(int));
    if (!new_mu) {                     /* allocation failed – give up */
        if (mu)
            free(mu);
        mu = NULL;
        mu_len = 0;
        return;
    }

    /* linear sieve for µ up to up_to */
    if (new_len > 1) {
        bool *is_comp = (bool*) calloc((size_t)new_len, sizeof(bool));
        int *primes   = (int*)  malloc((size_t)new_len * sizeof(int));
        size_t pcnt = 0;

        new_mu[1] = 1;
        for (idx_t i = 2; i < new_len; ++i) {
            if (!is_comp[i]) {
                new_mu[i] = -1;                 /* i is prime */
                primes[pcnt++] = (int)i;
            }
            for (size_t j = 0; j < pcnt; ++j) {
                idx_t v = (idx_t)primes[j] * i;
                if (v >= new_len) break;
                is_comp[v] = true;
                if (i % primes[j] == 0) {
                    new_mu[v] = 0;               /* square factor */
                    break;
                } else {
                    new_mu[v] = -new_mu[i];
                }
            }
        }
        free(is_comp);
        free(primes);
    }

    /* replace old array (if any) */
    if (mu)
        free(mu);
    mu = new_mu;
    mu_len = new_len;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    /* The sum runs over k = 1 … N, where N = n+1. */
    idx_t N = n + 1;
    ensure_mu(N);                 /* make sure µ(k) exists up to N */

    seq_t sum = 0;
    for (idx_t k = 1; k <= N; ++k) {
        int mk = mu[k];
        if (!mk)
            continue;              /* skip µ(k) = 0 */

        idx_t fl = N / k;
        idx_t ce = (N + k - 1) / k;   /* ceil division */

        seq_t term = (seq_t)fl * (seq_t)ce;
        if (mk > 0)
            sum += term;
        else
            sum -= term;
    }
    return sum;
}
