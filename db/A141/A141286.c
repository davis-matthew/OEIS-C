/*
 * A141286  a(n) = the smallest positive multiple of n such that a(n) is
 *          divisible by A001222(a(n)), where A001222(m) is the sum of the
 *          exponents in the prime factorization of m.
 * https://oeis.org/A141286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) is the smallest multiple m of n such that m % Ω(m) == 0,
 *            where Ω(m) is the sum of exponents in the prime factorization of
 *            m. The implementation searches incrementally for the smallest
 *            such m
 * Range:     u8 n <= 63, u16 n <= 6600, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Prime sieve up to MAX_PRIME. */
#define MAX_PRIME 1000000          /* upper bound for prime generation */
#define MAX_PRIMES 80000           /* number of primes ≤ MAX_PRIME (≈78498) */

static uint32_t  primes[MAX_PRIMES];
static size_t    primes_cnt = 0;
static bool      primes_inited = false;

static void init_primes(void)
{
    if (primes_inited)
        return;
    bool *is_prime = (bool *)malloc((MAX_PRIME + 1) * sizeof(bool));
    if (!is_prime)
        return; /* fallback: no primes generated */
    for (idx_t i = 2; i <= MAX_PRIME; ++i)
        is_prime[i] = true;
    for (idx_t i = 2; (unsigned __int128)i * i <= MAX_PRIME; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= MAX_PRIME; j += i)
                is_prime[j] = false;
        }
    }
    for (idx_t i = 2; i <= MAX_PRIME; ++i)
        if (is_prime[i]) {
            if (primes_cnt < MAX_PRIMES)
                primes[primes_cnt++] = (uint32_t)i;
        }
    free(is_prime);
    primes_inited = true;
}

/* Compute Ω(x) = sum of exponents in the prime factorization of x. */
static idx_t bigomega_idx(idx_t x)
{
    init_primes();
    idx_t cnt = 0;
    idx_t t   = x;
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = (idx_t)primes[i];
        if ((unsigned __int128)p * p > t)
            break;
        while (t % p == 0) {
            ++cnt;
            t /= p;
        }
    }
    if (t > 1)
        ++cnt;          /* remaining prime factor contributes one exponent */
    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Handle the case n == 0 gracefully (though not used). */
    if (n == 0)
        return (seq_t)0;

    idx_t k = 1;
    while (1) {
        idx_t m = n * k;      /* m = n * k; safe for n ≤ 10⁴ and small k */
        idx_t omega = bigomega_idx(m);
        if (omega && (m % omega == 0))
            return (seq_t)m;
        ++k;
    }
}
