/*
 * A062007  a(n) = mu(n)*prime(n).
 * https://oeis.org/A062007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = mu(n) * prime(n) (OEIS A062007)
 * Range:     i8 n <= 32, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"
#include "../utils/build_primes_upto.h"

static idx_t *spf = NULL;
static idx_t spf_limit = 0;

static uint64_t *prime_arr = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* Ensure smallest prime factor table covers [0..n] */
static void ensure_spf(idx_t n)
{
    if (n <= spf_limit)
        return;
    if (spf)
        free(spf);
    build_spf(n, &spf);
    spf_limit = n;
}

/* Ensure we have at least n primes generated */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;
    /* Grow the upper bound until we have enough primes */
    while (prime_cnt < (size_t)n) {
        idx_t limit = prime_limit ? (prime_limit * 2) : (n * 20 + 100);
        uint64_t *tmp;
        size_t cnt;
        build_primes_upto(limit, &tmp, &cnt);
        free(prime_arr);
        prime_arr = tmp;
        prime_cnt = cnt;
        prime_limit = limit;
    }
}

/* Compute Möbius of n using the SPF table */
static seq_t mu_of(idx_t n)
{
    idx_t t = n;
    int mu = 1;
    int k = 0;
    while (t > 1) {
        idx_t p = spf[t];
        int cnt = 0;
        while (t % p == 0) {
            t /= p;
            cnt++;
            if (cnt > 1)
                return (seq_t)0;            /* square factor → μ = 0 */
        }
        k++;
    }
    return (seq_t)(k % 2 ? -1 : 1);
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1, but guard anyway */
        return (seq_t)0;

    ensure_spf(n);               /* need SPF for μ */
    ensure_primes(n);            /* need nth prime */

    seq_t prime = (seq_t)prime_arr[n - 1];   /* 1‑based index */
    seq_t mu    = mu_of(n);
    return mu * prime;
}
