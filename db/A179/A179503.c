/*
 * A179503  a(n) = Sum_{k=0..n} C(n,k)*sigma(n,k)*sigma(n,n-k) for n>0 with
 *          a(0)=1.
 * https://oeis.org/A179503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)*sigma(n,k)*sigma(n,n-k) where
 *            sigma(n,k) is the sum of the k-th powers of the divisors of n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build system. */
extern seq_t seq_pow(seq_t, idx_t);  /* available helper for big integer exponent */

static const uint64_t SMALL_PRIMES[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97
};
static const size_t NUM_SMALL_PRIMES = sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]);

/* Factor n into primes stored in dest_pr[] and exponents in dest_ex[].
   Returns the number of distinct prime factors via *cnt. */
static void factorize_uint64(uint64_t n,
                             uint64_t *dest_pr, uint64_t *dest_ex, size_t *cnt)
{
    size_t c = 0;
    uint64_t m = n;
    for (size_t i = 0; i < NUM_SMALL_PRIMES && SMALL_PRIMES[i]*SMALL_PRIMES[i] <= m; ++i) {
        uint64_t p = SMALL_PRIMES[i];
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            dest_pr[c] = p;
            dest_ex[c] = e;
            ++c;
        }
    }
    if (m > 1) {            /* remaining prime factor */
        dest_pr[c] = m;
        dest_ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Enumerates all positive divisors of n into the array divs[], which has
   capacity >= tau and returns the count of divisors via *len. */
static void enumerate_divisors(uint64_t n,
                               const uint64_t *pr, const uint64_t *ex,
                               size_t pcnt,
                               uint64_t *divs, size_t *len)
{
    *len = 1;
    divs[0] = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        size_t base_len = *len;
        uint64_t powp = 1;
        for (uint64_t exp = 1; exp <= e; ++exp) {
            powp *= p;
            for (size_t j = 0; j < base_len; ++j) {
                divs[*len] = divs[j] * powp;
                ++(*len);
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;           /* a(0) = 1 */

    /* 1. Factor n */
    uint64_t pr[64], ex[64];
    size_t pcnt;
    factorize_uint64((uint64_t)n, pr, ex, &pcnt);

    /* 2. Enumerate divisors */
    /* tau <= 256 for n <= 10000, allocate ample space */
    uint64_t divs[256];
    size_t tau = 0;
    enumerate_divisors((uint64_t)n, pr, ex, pcnt, divs, &tau);

    /* 3. Compute sigma_k for k = 0..n */
    seq_t *sigma = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!sigma) return 0;          /* allocation failure */
    seq_t *pow_val = (seq_t *)malloc(tau * sizeof(seq_t));
    if (!pow_val) { free(sigma); return 0; }

    for (size_t j = 0; j < tau; ++j) pow_val[j] = 1;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t s = 0;
        for (size_t j = 0; j < tau; ++j) s += pow_val[j];
        sigma[k] = s;
        for (size_t j = 0; j < tau; ++j) pow_val[j] *= (seq_t)divs[j];
    }

    /* 4. Sum over k using binomial coefficients */
    seq_t res = 0;
    seq_t bin = 1;                 /* C(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = bin * sigma[k] * sigma[n - k];
        res += term;
        if (k < n) {
            bin = bin * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }

    free(pow_val);
    free(sigma);
    return res;
}
