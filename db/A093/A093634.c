/*
 * A093634  Primes of the form 9*10^k + R_k, where R_k is the repunit
 *          (A002275) of length k.
 * https://oeis.org/A093634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime of the form (82*10^k - 1)/9, i.e. primes
 *            of the form 9*10^k + R_k where R_k is the repunit of length k
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */

static idx_t next_k = 2;        /* next k to test (k = 2 gives 911) */
static seq_t pow10 = 100;       /* 10^next_k (initially 10^2) */

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> primes_vec;
#else
static seq_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;
#endif

static void init_primes(void)
{
#ifndef __cplusplus
    if (!primes)
    {
        primes_cap = 16;
        primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
        if (!primes) exit(EXIT_FAILURE);
    }
#endif
}

static void store_prime(seq_t p)
{
#ifdef __cplusplus
    primes_vec.push_back(p);
#else
    if (primes_cnt >= primes_cap)
    {
        size_t new_cap = primes_cap * 2;
        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_cnt++] = p;
#endif
}

static idx_t primes_count(void)
{
#ifdef __cplusplus
    return (idx_t)primes_vec.size();
#else
    return (idx_t)primes_cnt;
#endif
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    init_primes();

    /* Generate primes until we have at least n of them. */
    while (primes_count() < n)
    {
        /* cand = (82*10^k - 1)/9 */
        seq_t num = pow10 * (seq_t)82;
        num -= (seq_t)1;
        seq_t cand = num / (seq_t)9;

        if (is_prime_big(cand))
            store_prime(cand);

        /* advance to next k */
        pow10 *= (seq_t)10;
        ++next_k;
    }

    /* Return the n-th prime (1‑based). */
#ifdef __cplusplus
    return primes_vec[n - 1];
#else
    return primes[n - 1];
#endif
}
