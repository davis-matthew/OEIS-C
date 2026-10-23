/*
 * A130087  Denominator of product{k=1 to n} k^mu(k), where mu is the Moebius
 *          function A008683.
 * https://oeis.org/A130087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of ∏_{k=1..n} k^μ(k) where μ is the Möbius
 *            function. * The exponent of each prime p in the denominator is *
 *            sum_{i≥1} S(⌊n/p^i⌋) * where S(x)=∑_{k≤x} μ(k). Add the prime
 *            only when this sum is positive
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 52, u64 n <= 100,
 *            u128 n <= 190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Binary exponentiation for arbitrary‑precision seq_t */
static seq_t pow_big(seq_t base, uint64_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1ULL) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 1;

    /* Allocate SPFs, μ and cumulative sums */
    idx_t *spf = (idx_t*)calloc((size_t)(n + 1), sizeof(idx_t));
    int   *mu  = (int*)calloc((size_t)(n + 1), sizeof(int));
    int   *cum = (int*)calloc((size_t)(n + 1), sizeof(int));

    /* Linear sieve to fill spf and collect primes up to n */
    size_t primes_cap = 128;
    size_t primes_len = 0;
    uint32_t *primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));

    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            if (primes_len == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_len++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_len; ++j) {
            idx_t p = primes[j];
            if (p > spf[i] || (i * p) > n) break;
            spf[i * p] = p;
        }
    }

    /* Compute Möbius values */
    mu[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t p = spf[i];
        idx_t j = i / p;
        if (j % p == 0)     /* p² divides i → μ(i)=0 */
            mu[i] = 0;
        else                /* square‑free */
            mu[i] = -mu[j];
    }

    /* Cumulative sums S(x) */
    cum[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        cum[i] = cum[i - 1] + mu[i];

    /* Compute the denominator */
    seq_t result = 1;
    for (size_t idx = 0; idx < primes_len; ++idx) {
        idx_t p = primes[idx];
        int sum = 0;
        for (idx_t t = n / p; t > 0; t /= p)
            sum += cum[t];

        if (sum > 0)
            result *= pow_big((seq_t)p, (uint64_t)sum);
    }

    /* Free memory */
    free(spf);
    free(mu);
    free(cum);
    free(primes);

    return result;
}
