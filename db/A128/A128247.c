/*
 * A128247  a(n) = A001783(n)/A038610(n).
 * https://oeis.org/A128247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{1≤k≤n, gcd(k,n)=1} k / lcm_{1≤k≤n, gcd(k,n)=1} k
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 22, u64 n <= 30, u128 n <= 46,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* Count integers ≤ X that are coprime to n using
   inclusion–exclusion over all square‑free divisors of n.  */
static idx_t count_coprime(idx_t X, const uint64_t *divs, const int *mu, int divcnt)
{
    long long res = 0;
    for (int i = 0; i < divcnt; ++i)
        res += (long long)mu[i] * (X / divs[i]);
    return (idx_t)res;
}

seq_t a(idx_t n)
{
    /* Handle the trivial case. */
    if (n == 0)
        return seq_one();

    /* 1. Factor n and collect its distinct prime divisors. */
    uint64_t primes_n[16];
    uint64_t exps_n[16];
    size_t cnt_n;
    factor_u64((uint64_t)n, primes_n, exps_n, &cnt_n);

    /* 2. Build all square‑free divisors of n and their Möbius values. */
    uint64_t divs[64];
    int mu[64];
    int divcnt = 1;
    divs[0] = 1;
    mu[0] = 1;
    for (size_t i = 0; i < cnt_n; ++i) {
        int cur = divcnt;
        for (int j = 0; j < cur; ++j) {
            divs[divcnt] = divs[j] * primes_n[i];
            mu[divcnt] = -mu[j];
            ++divcnt;
        }
    }

    /* 3. Generate all primes up to n. */
    uint64_t *primes_all = NULL;
    size_t primes_all_cnt = 0;
    build_primes_upto(n, &primes_all, &primes_all_cnt);

    /* 4. Compute the product of prime powers. */
    seq_t result = seq_one();

    for (size_t i = 0; i < primes_all_cnt; ++i) {
        uint64_t p = primes_all[i];

        /* Skip primes that divide n. */
        bool divides = false;
        for (size_t j = 0; j < cnt_n; ++j) {
            if (p == primes_n[j]) {
                divides = true;
                break;
            }
        }
        if (divides)
            continue;

        /* Compute e_max = floor(log_p(n)). */
        uint64_t e_max = 0;
        uint64_t p_pow = p;
        while (p_pow <= n) {
            ++e_max;
            /* Avoid overflow for the next multiplication. */
            if (p_pow > n / p)
                break;
            p_pow *= p;
        }
        if (e_max == 0)
            continue;

        /* Compute exponent t_p = Σ_{k=1..e_max} count_coprime(n/p^k). */
        idx_t t_p = 0;
        p_pow = p;
        for (uint64_t e = 1; e <= e_max; ++e) {
            idx_t X = n / p_pow;
            t_p += count_coprime(X, divs, mu, divcnt);
            if (p_pow > n / p)
                break;
            p_pow *= p;
        }

        idx_t expo = t_p - (idx_t)e_max;
        if (expo > 0) {
            seq_t powv = seq_pow((seq_t)p, expo);
            result = seq_mul(result, powv);
        }
    }

    free(primes_all);
    return result;
}
