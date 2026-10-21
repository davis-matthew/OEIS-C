/*
 * A069249  a(n) = n^2 - phi(n)*sigma(n).
 * https://oeis.org/A069249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*n - phi(n)*sigma(n) (OEIS A069249)
 * Range:     u8 n <= 29, u16 n <= 461, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Required helpers provided by the framework */
#include "../utils/phi_local.h"

/* We implement our own very small power routine; no external helper for it.
   The exponent is always a small non‑negative integer (the exponent of a prime
   factor). */
static inline seq_t pow_seq_local(seq_t base, int exp)
{
    seq_t result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/* Prime list for factorisation.  We only need primes up to 10 000, which
   is enough for all n in the test suite (the maximum n tested is 10 000). */
static uint32_t primes[20000];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt != 0)
        return;                        /* already initialised */

    const idx_t limit = 10000;
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime)
        return;                        /* out of memory – should never happen */

    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    if (limit >= 0) {
        is_prime[0] = is_prime[1] = false;
    }
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }
    for (idx_t i = 2; i <= limit && prime_cnt < 20000; ++i) {
        if (is_prime[i])
            primes[prime_cnt++] = (uint32_t)i;
    }
    free(is_prime);
}

/* Sum of divisors σ(n) – factorises n with the pre‑computed prime list. */
static seq_t sigma_seq(seq_t n_seq)
{
    uint64_t n = (uint64_t)n_seq;
    if (n == 0)
        return 0;
    seq_t result = 1;
    uint64_t temp = n;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t p_seq = (seq_t)p;
            seq_t numerator = pow_seq_local(p_seq, exp + 1) - 1;
            seq_t denom = p_seq - 1;
            seq_t sigma_factor = numerator / denom;
            result *= sigma_factor;
        }
    }
    if (temp > 1) {                     /* remaining prime factor */
        seq_t p_seq = (seq_t)temp;
        seq_t sigma_factor = p_seq + 1;
        result *= sigma_factor;
    }
    return result;
}

/* Main sequence function (required by the framework) */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                        /* behaviour below offset is irrelevant */

    init_primes();                      /* ensure primes are ready */

    seq_t n_seq = (seq_t)n;
    seq_t phi = phi_local(n_seq);
    seq_t sigma = sigma_seq(n_seq);
    seq_t sq = n_seq * n_seq;
    seq_t prod = phi * sigma;
    return sq - prod;
}
