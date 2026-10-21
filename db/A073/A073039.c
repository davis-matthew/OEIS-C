/*
 * A073039  First multiple of n in A025487.
 * https://oeis.org/A073039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the lcm of the first e_k primes, where n = 2^e1 * 3^e2 *
 *            … * p_k^e_k. Equivalently, a(n) = ∏_{i=1}^k prime_i^{max(e_i,
 *            e_{i+1}, …, e_k)}
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   mul_pow_seqqt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_pow_seqqt.h"

/* ---------- prime sieve (up to 1,000,000) ---------- */
#define PRIME_SIEVE_LIMIT 1000000
static uint32_t *primes = NULL;
static size_t   prime_cnt = 0;
static bool     primes_ready = false;

static void init_primes(void)
{
    if (primes_ready)
        return;

    static bool *is_comp = NULL;
    is_comp = (bool *)malloc((PRIME_SIEVE_LIMIT + 1) * sizeof(bool));
    primes = (uint32_t *)malloc((PRIME_SIEVE_LIMIT) * sizeof(uint32_t)); /* enough space */

    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!is_comp[i])
        {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_SIEVE_LIMIT)
                for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    is_comp[j] = true;
        }

    free(is_comp);
    primes_ready = true;
}

/* ---------------------- main sequence function ---------------------- */
seq_t a(idx_t n)
{
    if (n < 2)           /* a(1) = 1 */
        return 1;

    init_primes();

    /* factorisation of n: store primes and exponents */
    static idx_t pf[64];
    static idx_t pf_exp[64];
    size_t pf_cnt = 0;

    idx_t tmp = n;

    /* factor 2 */
    if ((tmp & 1) == 0)
    {
        idx_t e = 0;
        while ((tmp & 1) == 0)
        {
            tmp >>= 1;
            ++e;
        }
        pf[pf_cnt] = 2;
        pf_exp[pf_cnt] = e;
        ++pf_cnt;
    }

    /* factor odd primes up to sqrt(tmp) */
    for (idx_t p = 3; (uint64_t)p * p <= (uint64_t)tmp; p += 2)
    {
        if (tmp % p == 0)
        {
            idx_t e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            pf[pf_cnt] = p;
            pf_exp[pf_cnt] = e;
            ++pf_cnt;
        }
    }

    /* leftover prime > sqrt(original n) */
    if (tmp > 1)
    {
        pf[pf_cnt] = tmp;
        pf_exp[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* maximum prime factor */
    idx_t max_prime = pf[pf_cnt - 1];
    size_t pf_idx = pf_cnt;   /* points to next unprocessed factor (from the end) */
    idx_t current_max_exp = 0;
    seq_t result = 1;

    /* if the largest prime exceeds our sieve, handle it first */
    if (max_prime > primes[prime_cnt - 1])
    {
        /* this prime is pf[pf_cnt-1] */
        current_max_exp = pf_exp[--pf_idx];
        mul_pow_seqqt(&result, (uint32_t)max_prime, current_max_exp);
    }

    /* iterate through all primes ≤ max_prime in descending order */
    for (int i = (int)prime_cnt - 1; i >= 0; --i)
    {
        uint32_t p = primes[i];
        if (p < 2 || p > max_prime)
            continue;

        idx_t e = 0;
        if (pf_idx > 0 && pf[pf_idx - 1] == p)
        {
            e = pf_exp[--pf_idx];
        }
        if (e > current_max_exp)
            current_max_exp = e;

        mul_pow_seqqt(&result, p, current_max_exp);
    }

    return result;
}
