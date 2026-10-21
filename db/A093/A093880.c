/*
 * A093880  a(n) = lcm(1, 2, ..., 2n) / lcm(1, 2, ..., n).
 * https://oeis.org/A093880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 21, u64 n <= 41, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A093880
    Formula:  a(n) = lcm(1,2,…,2n) / lcm(1,2,…,n)
    Computation:  For each prime p ≤ 2n, multiply a(n) by p
    to the power floor(log_p(2n)) – floor(log_p(n)).
    The needed primes are generated once and kept in a static table. */
#include <stdint.h>
#include <stdlib.h>

/* ---------- static prime cache ---------- */
static uint64_t *prime_tbl = NULL;   /* primes array */
static size_t   prime_cnt = 0;       /* number of primes stored */
static idx_t    prime_limit = 0;     /* largest n for which primes are known */

/* ---------- Sieve of Eratosthenes ----------
   Generates all primes ≤ limit and stores them in *out.
   For limit < 2 no primes are stored. */
static void generate_primes(idx_t limit, uint64_t **out, size_t *cnt)
{
    if (limit < 2) {
        *out  = NULL;
        *cnt  = 0;
        return;
    }

    size_t sz = (size_t)limit + 1;
    unsigned char *is_prime = (unsigned char *)malloc(sz);
    if (!is_prime) exit(1);

    for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
    is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (idx_t p = 2; (size_t)(p * p) <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;
        }
    }

    size_t npr = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++npr;

    uint64_t *pr = (uint64_t *)malloc(npr * sizeof(uint64_t));
    if (!pr) exit(1);

    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) pr[k++] = (uint64_t)i;

    free(is_prime);
    *out = pr;
    *cnt = npr;
}

/* ---------- Ensure prime list up to limit ----------
   Reallocates the cache if a larger limit is requested. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;

    if (prime_tbl) {
        free(prime_tbl);
        prime_tbl = NULL;
    }

    generate_primes(limit, &prime_tbl, &prime_cnt);
    prime_limit = limit;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;

    idx_t two_n = n * 2;
    ensure_primes(two_n);

    for (size_t idx = 0; idx < prime_cnt && prime_tbl[idx] <= two_n; ++idx) {
        uint64_t p = prime_tbl[idx];
        uint64_t power = p;

        /* power is now the first power > n */
        while (power <= n)
            power *= p;

        /* multiply result by all powers of p that do not exceed 2n */
        while (power <= two_n) {
            result *= (seq_t)p;
            power *= p;
        }
    }

    return result;
}
