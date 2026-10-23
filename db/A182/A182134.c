/*
 * A182134  Number of primes p such that prime(n) < p < prime(n)^(1 + 1/n).
 * https://oeis.org/A182134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primepi(prime(n)^(1+1/n)) - n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- static cache for sieve data ---------- */
static idx_t current_limit = 0;            /* max index sieved          */
static idx_t primes_cnt   = 0;             /* number of primes stored   */
static uint64_t *primes_arr = NULL;       /* primes[0] = 2             */
static idx_t *pi_arr       = NULL;        /* pi_arr[i] = #primes <= i  */

/* ---------- helpers ---------- */
static void recompute_sieve(idx_t limit)
{
    bool *is_prime = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_prime) return;

    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* count primes */
    idx_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *pr = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    idx_t *pi = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    if (!pr || !pi) {
        free(is_prime);
        free(pr); free(pi);
        return;
    }

    /* build primes[] and pi[] */
    idx_t pi_val = 0;
    idx_t p_idx = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (i >= 2 && is_prime[i]) {
            ++pi_val;
            pr[p_idx++] = (uint64_t)i;
        }
        pi[i] = pi_val;
    }

    free(is_prime);
    free(primes_arr); free(pi_arr);
    primes_arr = pr;
    pi_arr = pi;
    primes_cnt = cnt;
    current_limit = limit;
}

static void ensure_limit(idx_t limit)
{
    if (limit <= current_limit) return;
    recompute_sieve(limit);
}

static idx_t upper_bound_prime(idx_t n)
{
    if (n < 6) return 15;
    double nn = (double)n;
    double est = nn * (log(nn) + log(log(nn)));
    return (idx_t)(est) + 10;  /* safety margin */
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* out of OEIS range */

    /* ensure enough primes for index n */
    if (primes_cnt < n)
        ensure_limit(upper_bound_prime(n));
    while (primes_cnt < n)            /* safety loop if small overflow */
        ensure_limit(current_limit * 2 + 10);

    /* prime(n) */
    uint64_t p = primes_arr[n - 1];

    /* compute p^(1+1/n) accurately using long double */
    long double exponent = 1.0L + 1.0L / (long double)n;
    long double val = powl((long double)p, exponent);
    idx_t bound = (idx_t)(floor(val + 1e-12L));

    /* ensure sieve covers bound */
    if (bound > current_limit)
        ensure_limit(bound);

    /* result = pi(bound) - n */
    seq_t pi_at_bound = (seq_t)pi_arr[bound];
    seq_t result = pi_at_bound - (seq_t)n;
    return result;
}
