/*
 * A076078  a(n) is the number of nonempty sets of distinct positive integers
 *          that have a least common multiple of n.
 * https://oeis.org/A076078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = Sum_{m | n} mu(n/m) * (2^d(m) – 1)
 * Range:     u8 n <= 35, u16 n <= 179, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (d(m) = number of divisors of m, mu(k) = Möbius function) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* -------------------------------------------------------------------- */
/* Simple prime sieve – enough for n up to a few million (sqrt(n) ≤ 1000) */
/* -------------------------------------------------------------------- */
#define PRIME_LIMIT 200000            /* adjust if larger n is ever needed */
static uint64_t primes[200000];
static size_t prime_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void) {
    if (primes_initialized)
        return;

    static bool is_composite[PRIME_LIMIT + 1];
    size_t idx = 0;
    for (int i = 2; i <= PRIME_LIMIT; i++) {
        if (!is_composite[i]) {
            primes[idx++] = (uint64_t)i;
            /* mark multiples */
            if ((int64_t)i * i <= PRIME_LIMIT) {
                for (int j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    prime_cnt = idx;
    primes_initialized = true;
}

/* -------------------------------------------------------------------- */
/* Factorisation of n into primes[] / exps[] (prime factorisation)      */
/* -------------------------------------------------------------------- */
static size_t factorise(uint64_t n, uint64_t *primes_out,
                        int *exps_out) {
    size_t k = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; i++) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            primes_out[k] = p;
            exps_out[k] = e;
            k++;
        }
    }
    if (temp > 1) {              /* remaining prime factor > sqrt(n) */
        primes_out[k] = temp;
        exps_out[k] = 1;
        k++;
    }
    return k;
}

/* -------------------------------------------------------------------- */
/* Recursive enumeration of all divisors of n                           */
/* -------------------------------------------------------------------- */
static void dfs_divisors(size_t idx, size_t k, const int *full_exp,
                         int *curr_exp, __int128 *total) {
    if (idx == k) {
        /* number of divisors of m = product (curr_exp[j]+1)  */
        int dcnt = 1;
        /* Möbius of n/m : examine the remaining exponents */
        int mu = 1;                 /* start with sign +1 */
        for (size_t j = 0; j < k; j++) {
            int remain_exp = full_exp[j] - curr_exp[j];
            dcnt *= curr_exp[j] + 1;
            if (remain_exp >= 2) {   /* squared factor → μ = 0 */
                mu = 0;
                break;
            } else if (remain_exp == 1) {
                mu = -mu;            /* flip sign for each distinct prime */
            }
        }
        if (mu != 0) {
            /* compute 2^dcnt – 1 using 128‑bit arithmetic to avoid overflow */
            __int128 power = 1;
            for (int i = 0; i < dcnt; i++)
                power <<= 1;           /* multiply by 2 */
            __int128 term = power - 1;
            if (mu == 1)
                *total += term;
            else
                *total -= term;
        }
        return;
    }
    for (int e = 0; e <= full_exp[idx]; e++) {
        curr_exp[idx] = e;
        dfs_divisors(idx + 1, k, full_exp, curr_exp, total);
    }
}

/* -------------------------------------------------------------------- */
/* Public sequence function                                               */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0, but harmless */

    init_primes();

    uint64_t nv = (uint64_t)n;
    uint64_t prime_factors[64];
    int exp_factors[64];
    size_t k = factorise(nv, prime_factors, exp_factors);

    if (k == 0)            /* n == 1 */
        return (seq_t)1;

    int curr_exp[64];
    __int128 total = 0;
    dfs_divisors(0, k, exp_factors, curr_exp, &total);

    return (seq_t)total;
}
