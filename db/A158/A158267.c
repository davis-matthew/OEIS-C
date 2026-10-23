/*
 * A158267  Inverse Euler transform of A156305.
 * https://oeis.org/A158267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} sigma(d) * C(2*d-1, d) * mu(n/d)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom_2d_minus1_d
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here sigma is the sum of divisors function and μ is the Möbius function. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/binom_2d_minus1_d.h"

/* ------------------------------------------------------------ */
/* Small prime sieve (up to 20000 – enough for n <= 10000).    */
/* ------------------------------------------------------------ */
#define PRIME_LIMIT 20000
static uint32_t primes[PRIME_LIMIT];
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;

    static bool composite[PRIME_LIMIT + 1] = {0};
    composite[0] = composite[1] = true;
    for (size_t p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (!composite[p]) {
            for (size_t m = p * p; m <= PRIME_LIMIT; m += p)
                composite[m] = true;
        }
    }

    prime_cnt = 0;
    for (size_t p = 2; p <= PRIME_LIMIT; ++p)
        if (!composite[p])
            primes[prime_cnt++] = (uint32_t)p;

    primes_ready = true;
}

/* ------------------------------------------------------------ */
/* Integer square root (floor), by binary search.               */
/* ------------------------------------------------------------ */
static idx_t int_sqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid > 0 && mid > n / mid)  /* mid*mid > n ?  */
            high = mid - 1;
        else
            low  = mid + 1;
    }
    return high;
}

/* ------------------------------------------------------------ */
/* Sum‑of‑divisors function sigma(n).                          */
/* ------------------------------------------------------------ */
static void sigma_of(idx_t n, seq_t *sigma)
{
    seq_t result = 1;
    idx_t temp = n;
    for (size_t i = 0; i < prime_cnt && (idx_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            seq_t term = 1;      /* 1 + p + p^2 + ... */
            seq_t power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= (seq_t)p;
                term += power;
            }
            result *= term;
        }
    }
    if (temp > 1) {            /* remaining prime factor */
        result *= (seq_t)(1 + temp);
    }
    *sigma = result;
}

/* ------------------------------------------------------------ */
/* Möbius function μ(n).                                       */
/* ------------------------------------------------------------ */
static void mobius_of(idx_t n, int *mu)
{
    int m = 1;
    idx_t temp = n;
    for (size_t i = 0; i < prime_cnt && (idx_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
                if (exp > 1) {   /* square factor → μ = 0 */
                    *mu = 0;
                    return;
                }
            }
            m = -m;
        }
    }
    if (temp > 1)               /* remaining prime factor */
        m = -m;
    *mu = m;
}

/* ------------------------------------------------------------ */
/* Binomial coefficient C(2d-1, d) computed multiplicatively. */

/* ------------------------------------------------------------ */
/* Main OEIS sequence function.                                */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; safety guard */

    init_primes();

    seq_t pos = 0, neg = 0;
    idx_t r = int_sqrt(n);

    for (idx_t i = 1; i <= r; ++i) {
        if (n % i != 0) continue;

        idx_t d1 = i;
        idx_t d2 = n / i;

        /* first divisor d1 ------------------------------------- */
        seq_t sigma1, comb1;
        int mu2;
        sigma_of(d1, &sigma1);
        comb1 = binom_2d_minus1_d(d1);
        mobius_of(d2, &mu2);
        seq_t term1 = sigma1 * comb1;
        if (mu2 == 1) pos += term1;
        else if (mu2 == -1) neg += term1;

        /* second divisor d2 (if different) ---------------------- */
        if (d1 != d2) {
            seq_t sigma2, comb2;
            int mu1;
            sigma_of(d2, &sigma2);
            comb2 = binom_2d_minus1_d(d2);
            mobius_of(d1, &mu1);
            seq_t term2 = sigma2 * comb2;
            if (mu1 == 1) pos += term2;
            else if (mu1 == -1) neg += term2;
        }
    }

    /* pos ≥ neg for the signed sum of the formula. */
    seq_t sum = pos - neg;
    return sum / (seq_t)n;
}
