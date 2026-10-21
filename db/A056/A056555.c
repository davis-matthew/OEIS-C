/*
 * A056555  Smallest number k (k>0) such that n*k is a perfect 4th power.
 * https://oeis.org/A056555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p|n} p^{(4 - ord_p(n)) mod 4} (multiplicative)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Prime sieve (global static data) */
#define SIEVE_MAX 2000000          /* enough to factor numbers up to ~10^12   */
static unsigned char sieve_composite[SIEVE_MAX + 1];
static int sieve_primes[150000];
static int sieve_prime_cnt = 0;
static bool sieve_done = false;

static void init_sieve(void)
{
    int i, j;
    for (i = 0; i <= SIEVE_MAX; ++i) sieve_composite[i] = 0;
    for (i = 2; i * i <= SIEVE_MAX; ++i) {
        if (!sieve_composite[i]) {
            for (j = i * i; j <= SIEVE_MAX; j += i) sieve_composite[j] = 1;
        }
    }
    for (i = 2; i <= SIEVE_MAX; ++i) {
        if (!sieve_composite[i]) sieve_primes[sieve_prime_cnt++] = i;
    }
    sieve_done = true;
}

/* Factorization routine using pre‑computed primes */
static void factor_uint64(uint64_t n,
                          uint64_t *pf,    /* primes  (size <= 64) */
                          uint32_t *pe,    /* exponents (size <= 64) */
                          size_t *cnt)
{
    size_t i, c = 0;
    uint64_t tmp = n;
    if (!sieve_done) init_sieve();

    for (i = 0; i < (size_t)sieve_prime_cnt && (uint64_t)sieve_primes[i] * (uint64_t)sieve_primes[i] <= tmp; ++i) {
        uint64_t p = (uint64_t)sieve_primes[i];
        if (tmp % p == 0) {
            uint32_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            pf[c] = p;
            pe[c] = exp;
            ++c;
        }
    }
    if (tmp > 1) {          /* remaining prime > SIEVE_MAX (or n itself) */
        pf[c] = tmp;
        pe[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but guard against overflow */
    uint64_t num = (uint64_t)n;

    uint64_t primes_arr[64];
    uint32_t exps_arr[64];
    size_t prime_cnt = 0;
    factor_uint64(num, primes_arr, exps_arr, &prime_cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t rem = (4 - (exps_arr[i] % 4)) % 4;   /* (4 - exp) mod 4 */
        while (rem--) {
            result *= (seq_t)primes_arr[i];
        }
    }
    return result;
}
