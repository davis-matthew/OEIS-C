/*
 * A081535  Sum of row n of triangle in A081536.
 * https://oeis.org/A081535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= n*(n+1)/2 with k - gpp(k) >= n*(n-1)/2 * (for
 *            n <= 4 the values are defined explicitly: * a(1)=1, a(2)=0,
 *            a(3)=6, a(4)=14)
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Prime sieve – gives a list of all primes up to 200 000.               */
static uint32_t *primes = NULL;
static size_t   prime_cnt = 0;

static void
init_primes(void)
{
    if (primes != NULL)      /* already initialised */
        return;

    /* 200 000 is more than enough for sqrt(5×10^7) (the size of a(10000)) */
    const size_t limit = 200000;
    bool *is_prime = (bool*)malloc(limit + 1);
    for (size_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* count primes */
    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++prime_cnt;

    primes = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes[pos++] = (uint32_t)i;

    free(is_prime);
}

/* --------------------------------------------------------------------- */
/* Largest prime‑power divisor of a 64‑bit integer.                       */
static seq_t
largest_prime_power_uint64(uint64_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t m      = n;
    uint64_t max_pow = 1;

    for (size_t i = 0; i < prime_cnt &&
         (uint64_t)primes[i] * (uint64_t)primes[i] <= m; ++i) {

        uint32_t p = primes[i];
        if (m % p == 0) {
            uint64_t pow = 1;
            while (m % p == 0) {
                m /= p;
                pow *= p;
            }
            if (pow > max_pow)
                max_pow = pow;
        }
    }

    /* remaining factor > sqrt(m) (if any) is prime with exponent 1   */
    if (m > 1 && m > max_pow)
        max_pow = m;

    return (seq_t)max_pow;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                               */
seq_t
a(idx_t n)
{
    /* Explicit values for the first four indices                      */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)14;

    init_primes();

    seq_t lower  = (seq_t)n * ((seq_t)n + 1) / 2;   /* n(n+1)/2             */
    seq_t target = (seq_t)n * ((seq_t)n - 1) / 2;   /* n(n-1)/2             */

    for (uint64_t k = (uint64_t)lower; ; ++k) {
        seq_t kk   = (seq_t)k;
        seq_t gpp  = largest_prime_power_uint64(k);
        if (kk - gpp >= target)
            return kk;
    }
}
