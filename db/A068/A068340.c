/*
 * A068340  a(n) = Sum_{k=1..n} mu(k)*k, where mu(k) is the Moebius function.
 * https://oeis.org/A068340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} mu(k) * k
 * Range:     i8 n <= 72, i16 n <= 2650, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Memoise the cumulative sum of mu(k)*k for speed. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static idx_t max_n = 0;
static int8_t *mu_arr = NULL;
static seq_t *cum_arr = NULL;

/* Ensure we have computed mu(k) and cumulative sums up to `n`. */
static void ensure_up_to(idx_t n)
{
    if (n <= max_n)
        return;

    /* Allocate new arrays.  (A realloc would also work.) */
    int8_t  *new_mu = (int8_t *)malloc(sizeof(int8_t) * (n + 1));
    seq_t   *new_cum = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!new_mu || !new_cum) {
        /* In a real program you'd handle this better. */
        exit(1);
    }

    /* Initialise mu(1)=1 and mu(k)=1 for k>=2. */
    if (n >= 1) new_mu[1] = 1;
    for (idx_t i = 2; i <= n; ++i)
        new_mu[i] = 1;

    /* Sieve to compute the Möbius function. */
    uint8_t *is_prime = (uint8_t *)malloc(n + 1);
    memset(is_prime, 1, n + 1);
    if (n >= 0) is_prime[0] = 0;
    if (n >= 1) is_prime[1] = 0;

    for (idx_t i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            /* flip sign for every multiple of the prime */
            for (idx_t j = i; j <= n; j += i) {
                new_mu[j] = -new_mu[j];
                is_prime[j] = 0; /* mark composite so we skip it later */
            }
            /* zero out multiples of p^2 to handle squared prime factors */
            for (idx_t j = i * i; j <= n; j += i * i)
                new_mu[j] = 0;
        }
    }
    free(is_prime);

    /* Build cumulative sums a(n) = Σ_{k=1..n} mu(k)*k. */
    new_cum[0] = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        new_cum[i] = new_cum[i - 1] + (seq_t)new_mu[i] * (seq_t)i;
    }

    /* Replace old arrays with the new ones. */
    free(mu_arr);
    free(cum_arr);
    mu_arr = new_mu;
    cum_arr = new_cum;
    max_n = n;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    ensure_up_to(n);
    return cum_arr[n];
}
