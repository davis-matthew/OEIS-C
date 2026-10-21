/*
 * A052297  Number of distinct prime factors of all composite numbers between
 *          n-th and (n+1)st primes.
 * https://oeis.org/A052297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{p ≤ P_n : ∃ composite x with P_n < x < P_{n+1} and p |
 *            x}|
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

static idx_t estimate_limit(idx_t n)
/* Rosser–Schoenfeld upper bound for the n‑th prime, valid for n ≥ 6. */
{
    if (n < 6) return 15;
    double dn = (double)n;
    double est = dn * (log(dn) + log(log(dn)));
    return (idx_t)(est + 3.0);
}

static void sieve_primes(idx_t limit, idx_t **primes, size_t *cnt)
{
    size_t size = (size_t)limit + 1;
    bool *is_comp = (bool *)calloc(size, sizeof(bool));
    if (!is_comp) { *primes = NULL; *cnt = 0; return; }

    /* Rough estimate of the number of primes for allocation. */
    size_t alloc = limit / 2 + 1;
    idx_t *buf = (idx_t *)malloc(alloc * sizeof(idx_t));
    if (!buf) { free(is_comp); *primes = NULL; *cnt = 0; return; }

    size_t c = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[(size_t)i]) {
            if (c == alloc) {
                alloc *= 2;
                idx_t *tmp = (idx_t *)realloc(buf, alloc * sizeof(idx_t));
                if (!tmp) { free(is_comp); free(buf); *primes = NULL; *cnt = 0; return; }
                buf = tmp;
            }
            buf[c++] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[(size_t)j] = true;
            }
        }
    }
    free(is_comp);

    idx_t *res = (idx_t *)realloc(buf, c * sizeof(idx_t));
    if (!res) res = buf;          /* if realloc fails keep the original pointer */
    *primes = res;
    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Compute enough primes to access P_n and P_{n+1}. */
    idx_t limit = estimate_limit(n + 1);
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    sieve_primes(limit, &primes, &prime_cnt);

    /* In the tiny cases the Rosser bound may be too low; enlarge if needed. */
    while (prime_cnt <= n) {
        limit = estimate_limit(limit * 2);
        free(primes);
        sieve_primes(limit, &primes, &prime_cnt);
    }

    idx_t Pn     = primes[n - 1];
    idx_t Pnext  = primes[n ];

    seq_t cnt = 0;
    for (size_t i = 0; i < prime_cnt && primes[i] <= Pn; ++i) {
        idx_t p = primes[i];
        idx_t m = ((Pn / p) + 1) * p;   /* first multiple of p strictly greater than Pn */
        if (m < Pnext) ++cnt;
    }

    free(primes);
    return cnt;
}
