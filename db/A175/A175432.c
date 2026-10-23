/*
 * A175432  a(n) = the greatest number k such that sigma(n) = m^k for any m >=
 *          1 (sigma = A000203).
 * https://oeis.org/A175432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the greatest k such that sigma(n) = m^k for some m ≥ 1
 *            (sigma = A000203, the sum of divisors)
 * Range:     u8 n <= 30, u16 n <= 9773, u32 n <= 9773, u64 n <= 9773,
 *            u128 n <= 9773, big any n
 * Helpers:   ugcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ugcd_uint64.h"

/* Utility: gcd for 64‑bit unsigned (used only internally) */

/* Precompute a list of primes up to 1000 (enough for n ≤ 10000 and
   for factoring sigma(n)).  The array is filled once on the first call. */
static uint64_t primes[200];
static size_t prime_cnt = 0;
static bool primes_ready = false;
static void build_primes(void)
{
    if (primes_ready)
        return;
    /* Simple sieve up to 1000 */
    bool is_composite[1001] = { false };
    for (uint32_t i = 2; i <= 1000; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= 1000) {
                for (uint32_t j = i * i; j <= 1000; j += i)
                    is_composite[j] = true;
            }
        }
    }
    primes_ready = true;
}

/* Factorization of a 64‑bit integer into primes and exponents.
   The result is written to 'pr' and 'ex', and the number of distinct
   prime factors is stored in '*cnt'.  The caller must provide
   arrays large enough (≥ number of distinct primes). */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t pos = 0;
    if (n <= 1) {
        *cnt = 0;
        return;
    }

    if (n % 2 == 0) {
        uint64_t p = 2, e = 0;
        while (n % 2 == 0) {
            n /= 2;
            ++e;
        }
        pr[pos] = p;
        ex[pos] = e;
        ++pos;
    }

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= n; ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[pos] = p;
            ex[pos] = e;
            ++pos;
        }
    }

    if (n > 1) {            /* n is now prime */
        pr[pos] = n;
        ex[pos] = 1;
        ++pos;
    }

    *cnt = pos;
}

/* Compute sigma(n) = product over primes p^e of (p^(e+1)-1)/(p-1)  */
static seq_t sigma_of_uint64(uint64_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_uint64(n, pr, ex, &cnt);
    if (cnt == 0)       /* n == 1, sigma = 1 */
        return 1;

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        /* compute p^(e+1) */
        seq_t power = 1;
        for (uint64_t k = 0; k < e + 1; ++k)
            power *= p;
        seq_t term = (power - 1) / (p - 1);
        result *= term;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1 */

    build_primes();       /* ensure primes are available */

    /* 1. compute sigma(n) */
    seq_t sigma = sigma_of_uint64((uint64_t)n);

    /* 2. factor sigma to get exponents */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_uint64((uint64_t)sigma, pr, ex, &cnt);

    /* 3. greatest common divisor of all exponents */
    uint64_t k = 1;
    if (cnt > 0) {
        k = ex[0];
        for (size_t i = 1; i < cnt; ++i)
            k = ugcd_uint64(k, ex[i]);
    }

    return (seq_t)k;      /* k is at least 1 for all n ≥ 1 */
}
