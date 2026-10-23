/*
 * A023818  Sum of exponents in prime-power factorization of C(2n,n-2).
 * https://oeis.org/A023818
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of exponents in prime‑power factorization of
 *            C(2n,n-2)
 * Range:     u8 n <= 1184, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Dynamic prime table */
static uint64_t *g_primes   = NULL; /* array of primes */
static size_t   g_prime_cnt = 0;    /* current number of primes stored */
static size_t   g_prime_cap = 0;    /* allocated capacity of g_primes */
static idx_t    g_prime_lim = 1;    /* largest number to which we have sieved */

/* Ensure that primes up to at least 'limit' are available. */
static void ensure_primes(idx_t limit)
{
    if (limit <= g_prime_lim)                /* already done */
        return;

    /* Grow primes array if necessary */
    size_t need = g_prime_cnt + (limit - g_prime_lim);
    if (need > g_prime_cap) {
        size_t new_cap = g_prime_cap ? g_prime_cap * 2 : need;
        if (new_cap < need) new_cap = need;
#if defined(__cplusplus)
        uint64_t *tmp = reinterpret_cast<uint64_t *>(
                            realloc(g_primes, new_cap * sizeof(uint64_t)));
#else
        uint64_t *tmp = (uint64_t *)realloc(g_primes, new_cap * sizeof(uint64_t));
#endif
        if (!tmp) exit(EXIT_FAILURE);
        g_primes   = tmp;
        g_prime_cap = new_cap;
    }

    /* Incrementally find primes from g_prime_lim+1 up to limit */
    for (idx_t num = g_prime_lim + 1; num <= limit; ++num) {
        bool is_prime = true;
        for (size_t i = 0; i < g_prime_cnt; ++i) {
            uint64_t p = g_primes[i];
            if (p > num / p)                 /* p > sqrt(num) */
                break;
            if (num % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            g_primes[g_prime_cnt++] = num;
    }

    g_prime_lim = limit;
}

seq_t a(idx_t n)
{
    if (n < 2)                 /* sequence is defined for n >= 2 */
        return 0;

    idx_t two_n     = 2 * n;
    idx_t n_minus_2 = n - 2;
    idx_t n_plus_2  = n + 2;

    /* Ensure we have primes up to 2*n */
    ensure_primes(two_n);

    /* Sum of exponents in C(2n,n-2) */
    uint64_t sum = 0;
    for (size_t i = 0; i < g_prime_cnt; ++i) {
        uint64_t p = g_primes[i];
        if (p > two_n)
            break;

        /* exponent of p in (2n)! */
        uint64_t e2 = 0;
        idx_t t = two_n;
        while (t) { t /= p; e2 += t; }

        /* exponent of p in (n-2)! */
        uint64_t e1 = 0;
        t = n_minus_2;
        while (t) { t /= p; e1 += t; }

        /* exponent of p in (n+2)! */
        uint64_t e3 = 0;
        t = n_plus_2;
        while (t) { t /= p; e3 += t; }

        sum += e2 - e1 - e3;
    }

    return (seq_t)sum;
}
