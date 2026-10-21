/*
 * A099611  a(n) is the largest odd number that is less than n^2 and is the
 *          product of two distinct primes.
 * https://oeis.org/A099611
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest odd m < n^2 such that m is a product of two
 *            distinct primes
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* global prime table */
static uint64_t *s_primes = NULL;
static size_t   s_prime_count = 0;
static idx_t    s_prime_limit = 0;

/* make sure s_primes contains all primes up to 'limit' */
static void ensure_primes(idx_t limit)
{
    if (s_prime_limit >= limit)
        return;
    if (s_primes) {
        free(s_primes);
        s_primes = NULL;
    }
    bool *comp = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!comp)
        return;                      /* OOM – caller will handle failure */
    for (idx_t i = 0; i <= limit; ++i)
        comp[i] = false;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!comp[p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                comp[q] = true;
        }
    }
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[i])
            ++cnt;
    s_primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!s_primes) {
        free(comp);
        return;
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[i])
            s_primes[idx++] = (uint64_t)i;
    s_prime_count = cnt;
    s_prime_limit = limit;
    free(comp);
}

/* test whether m is a product of two distinct odd primes */
static bool is_semiprime_distinct(idx_t m)
{
    if (m < 9)                     /* smallest allowed is 3*5 */
        return false;
    for (size_t i = 0; i < s_prime_count; ++i) {
        uint64_t p = s_primes[i];
        if (p == 2)
            continue;             /* 2 is not allowed */
        idx_t pp = (idx_t)p;
        if (pp * pp > m)
            break;
        if (m % pp == 0) {
            idx_t q = m / pp;
            if (q == pp)         /* distinct primes */
                continue;
            if (is_prime(q))
                return true;
        }
    }
    return false;
}

/* required sequence function */
seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;          /* value undefined for n < 4 */
    /* primes up to n-1 are enough (sqrt(m) < n) */
    ensure_primes(n - 1);
    idx_t n2 = (idx_t)n * (idx_t)n;
    idx_t m = n2 - 1;
    if ((m & 1) == 0)
        --m;                      /* make sure we start with an odd number */
    for (idx_t cur = m; cur >= 3; cur -= 2) {
        if (is_semiprime_distinct(cur))
            return (seq_t)cur;
    }
    return (seq_t)0;              /* should never happen */
}
