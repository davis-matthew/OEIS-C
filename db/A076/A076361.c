/*
 * A076361  Numbers k such that d(sigma(k)) = sigma(d(k)).
 * https://oeis.org/A076361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 829, u32 n <= 9800, u64 n <= 9800,
 *            u128 n <= 9800, big any n
 * Helpers:   sigma_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076361: Numbers k such that d(sigma(k)) = sigma(d(k)).
   Computation is done on demand: each call to a(n) generates all terms
   up to n if not already known.  The algorithm checks every integer k
   in ascending order, computes sigma(k) and tau(k), factors
   sigma(k), and verifies the equality. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/sigma_small.h"

/*---  Prime table (up to 1 000 000) --------------------------------------*/
#define PRIME_LIMIT  1000000
#define PRIME_CAP    120000   /* enough for all primes ≤ 1 000 000 */

static uint32_t prime_tbl[PRIME_CAP];
static size_t   prime_cnt = 0;
static bool     primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;

    uint8_t *is_comp = (uint8_t*)malloc(PRIME_LIMIT + 1);
    memset(is_comp, 0, PRIME_LIMIT + 1);

    size_t cnt = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            prime_tbl[cnt++] = (uint32_t)i;
            if ((int64_t)i * i <= PRIME_LIMIT) {
                for (int j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    prime_cnt = cnt;
    free(is_comp);
}

/*---  Factorisation routine (returns prime factors and exponents) --------*/
static void factor_uint64(uint64_t n, uint64_t *factors, int *exps, size_t *out_cnt)
{
    size_t cnt = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_tbl[i] * prime_tbl[i] <= n; ++i) {
        uint32_t p = prime_tbl[i];
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            factors[cnt]   = (uint64_t)p;
            exps[cnt]      = e;
            ++cnt;
        }
    }
    if (n > 1) {                     /* remaining prime factor > sqrt(original n) */
        factors[cnt]   = n;
        exps[cnt]      = 1;
        ++cnt;
    }
    *out_cnt = cnt;
}

/*---  Test whether the equality holds for a given k ----------------------*/
static bool test_k(uint64_t k)
{
    /* factor k */
    uint64_t f[64];
    int     e[64];
    size_t  cnt;
    factor_uint64(k, f, e, &cnt);

    /* compute tau(k) and sigma(k) */
    uint64_t tau_k   = 1;
    __uint128_t sigma_k128 = 1;   /* use 128‑bit to avoid overflow of sigma(k) */
    for (size_t i = 0; i < cnt; ++i) {
        tau_k *= (uint64_t)(e[i] + 1);

        /* 1 + p + p^2 + … + p^e[i] */
        uint64_t sum = 1;
        uint64_t term = 1;
        for (int exp = 1; exp <= e[i]; ++exp) {
            term *= f[i];
            sum += term;
        }
        sigma_k128 *= sum;
    }

    /* sigma(tau(k)) */
    uint64_t sigma_tau = sigma_small(tau_k);

    /* tau(sigma(k)) – need to factor sigma(k) */
    uint64_t sigma_k = (uint64_t)sigma_k128;   /* assume it fits in 64‑bit for the test */
    uint64_t f2[64];
    int     e2[64];
    size_t  cnt2;
    factor_uint64(sigma_k, f2, e2, &cnt2);
    uint64_t tau_sigma = 1;
    for (size_t i = 0; i < cnt2; ++i)
        tau_sigma *= (uint64_t)(e2[i] + 1);

    return tau_sigma == sigma_tau;
}

/*---  Memoised sequence generator ----------------------------------------*/
seq_t a(idx_t n)
{
    static seq_t *list   = NULL;
    static size_t list_sz = 0;
    static size_t list_cap = 0;
    static uint64_t next_k = 1;

    /* initialise primes once */
    if (!primes_inited) init_primes();

    /* initialise the list once */
    if (!list) {
        list_cap = 256;
        list = (seq_t*)malloc(list_cap * sizeof(seq_t));
    }

    /* generate terms until we have n of them */
    while (list_sz < n) {
        if (test_k(next_k)) {
            if (list_sz >= list_cap) {
                list_cap *= 2;
                list = (seq_t*)realloc(list, list_cap * sizeof(seq_t));
            }
            list[list_sz++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return list[n - 1];
}
