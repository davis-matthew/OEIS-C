/*
 * A033469  Denominator of Bernoulli(2n,1/2).
 * https://oeis.org/A033469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator of Bernoulli(2n,1/2) * = (∏_{p-1 | 2n} p) *
 *            2^(2n-1) / gcd(∏_{p-1 | 2n} p , 2^(2n-1)-1) * where the product
 *            runs over all primes p with (p-1) dividing 2n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   _powmod2_uint64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>               /* for bool type */
#include "../utils/seq_pow.h"      /* seq_pow(seq_t base, idx_t exp) */
#include "../utils/_powmod2_uint64.h"

/* ---------- Prime sieve (once) ---------- */
#define PRIME_LIMIT 20005         /* > 2*n+1 for n up to 10000 */
static uint32_t _primes[PRIME_LIMIT / 2 + 10];
static size_t _prime_count = 0;
static bool _primes_initialized = false;

static void _init_primes(void)
{
    if (_primes_initialized) return;
    _primes_initialized = true;
    bool *is_prime = (bool *)malloc(sizeof(bool) * PRIME_LIMIT);
    for (size_t i = 0; i < PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i < PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j < PRIME_LIMIT; j += i)
                is_prime[j] = false;
    for (size_t i = 2; i < PRIME_LIMIT; ++i)
        if (is_prime[i])
            _primes[_prime_count++] = (uint32_t)i;
    free(is_prime);
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    _init_primes();

    idx_t m = 2 * n;                /* 2n */
    seq_t denom = 1;                /* product of primes p with p-1 | m (B_{2n} denominator) */
    seq_t g = 1;                    /* gcd part */

    for (size_t ip = 0; ip < _prime_count; ++ip) {
        uint32_t p = _primes[ip];
        if ((idx_t)p > m + 1) break;
        if (m % ((idx_t)p - 1) == 0) {
            denom *= (seq_t)p;      /* multiply prime into denominator */
            /* test whether p | (2^(m-1) - 1) */
            if (_powmod2_uint64(m - 1, p) == 1)
                g *= (seq_t)p;      /* prime contributes to gcd */
        }
    }

    seq_t pow2 = seq_pow((seq_t)2, m - 1); /* 2^(2n-1) */
    seq_t denom_div_g = denom / g;         /* B / gcd */
    return denom_div_g * pow2;             /* final answer */
}
