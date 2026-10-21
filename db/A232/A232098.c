/*
 * A232098  a(n) is the largest m such that m! divides n^2; a(n) =
 *          A055881(n^2).
 * https://oeis.org/A232098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest m such that m! divides n^2 (A232098)
 * Range:     u8 n <= 7755, u16 n <= 7755, u32 n <= 7755, u64 n <= 7755,
 *            u128 n <= 7755, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"

/* Small prime table for checking primes up to a safe bound (2000). */
static uint64_t primes_small[400];
static size_t primes_small_cnt = 0;
static void init_primes_small(void)
{
    if (primes_small_cnt != 0) return;          /* already initialized */
    const idx_t LIMIT = 2000;
    bool sieve[LIMIT + 1];
    memset(sieve, 0, sizeof(sieve));
    primes_small_cnt = 0;
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (!sieve[p]) {
            primes_small[primes_small_cnt++] = p;
            if (p * p <= LIMIT)
                for (idx_t q = p * p; q <= LIMIT; q += p)
                    sieve[q] = true;
        }
    }
}

/* Exponent of prime p in m!  (Legendre's formula). */
static long long exp_in_fact(idx_t m, idx_t p)
{
    long long exp = 0;
    for (uint64_t pow = p; pow <= m; pow *= p) {
        exp += m / pow;
        if (pow > m / p) break;   /* avoid overflow */
    }
    return exp;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure the small prime table is ready. */
    init_primes_small();

    /* Factorize n and store prime^exponent pairs. */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Array of target exponents in n^2 (2 * exponent in n). */
    long long target[64];
    for (size_t i = 0; i < cnt; ++i)
        target[i] = 2LL * (long long)ex[i];

    seq_t m = 1;   /* 1! always divides n^2. */

    for (idx_t nxt = 2;; ++nxt) {
        bool ok = true;

        /* For every prime <= nxt, check the factorial exponent. */
        for (size_t i = 0; i < primes_small_cnt && primes_small[i] <= nxt; ++i) {
            idx_t p = primes_small[i];
            long long need = 0;      /* default: p does not divide n^2 */

            for (size_t j = 0; j < cnt; ++j)
                if (pr[j] == p) { need = target[j]; break; }

            long long have = exp_in_fact(nxt, p);
            if (have > need) { ok = false; break; }
        }

        if (!ok) return m;
        m = nxt;
    }
}
