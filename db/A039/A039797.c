/*
 * A039797  Triangle of numbers of Dyck paths.
 * https://oeis.org/A039797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   T(n,k) = (2n-k)!*(2n-k+2)!*(k+3)! /
 *            ((n-k)!*(n-k+1)!*k!*(n+2)!*(n+3)!) for 0 <= k <= n a(n) is
 *            obtained by flattening the triangle row‑by‑row: a(n) = T(m,k)
 *            where m(m+1)/2 <= n < (m+1)(m+2)/2 and k = n - m(m+1)/2
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   build_primes, pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes.h" /* build_primes(limit, &primes, &cnt) */
#include "../utils/pow_seq.h"     /* pow_seq(seq_t base, int exp) */
#include "../utils/seq_mul.h"     /* seq_mul(seq_t a, seq_t b) */

static idx_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t prime_cache_limit = 0;

/* exponent of prime p in m!  (Legendre's formula) */
static inline idx_t exp_factorial(idx_t m, idx_t p)
{
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

/* ensure prime_cache contains all primes <= limit */
static void ensure_primes(idx_t limit)
{
    if (prime_cache_limit >= limit)
        return;
    free(prime_cache);
    build_primes(limit, &prime_cache, &prime_cache_cnt);
    prime_cache_limit = limit;
}

seq_t a(idx_t n)
{
    /* find row m containing index n */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n)
        ++m;
    idx_t k = n - m * (m + 1) / 2;

    /* maximum value needed for factorial arguments */
    idx_t max_val = (2 * m - k + 2 > k + 3 ? 2 * m - k + 2 : k + 3);
    if (m + 3 > max_val)
        max_val = m + 3;
    if (m + 2 > max_val)
        max_val = m + 2;
    ensure_primes(max_val);

    seq_t res = 1;

    for (size_t idx = 0; idx < prime_cache_cnt; ++idx) {
        idx_t p = prime_cache[idx];
        if (p > max_val)
            break;
        int64_t e =
            (int64_t)exp_factorial(2 * m - k, p) +
            (int64_t)exp_factorial(2 * m - k + 2, p) +
            (int64_t)exp_factorial(k + 3, p) -
            (int64_t)exp_factorial(m - k, p) -
            (int64_t)exp_factorial(m - k + 1, p) -
            (int64_t)exp_factorial(k, p) -
            (int64_t)exp_factorial(m + 2, p) -
            (int64_t)exp_factorial(m + 3, p);

        if (e > 0) {
            seq_t power = pow_seq((seq_t)p, (int)e);
            res = seq_mul(res, power);
        }
    }

    return res;
}
