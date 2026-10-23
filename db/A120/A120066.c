/*
 * A120066  (n-1)! divided by (product phi(d)! ; d divides n).
 * https://oeis.org/A120066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 23, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   divisors_from_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A120066
   a(n) = (n-1)! / ∏_{d|n} φ(d)!   ,  n ≥ 1
   where φ is Euler's totient.  The function uses prime‑factor
   exponents to keep values exact and small.  It compiles under C11
   and C++17 (no <complex> etc.).  A small internal sieve generates
   primes up to n-1; all other helpers are defined locally. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/divisors_from_factor.h"

/* ------------------------------------------------------------------ */
/* Sieve of Eratosthenes – primes ≤ limit.  Returned array is
   allocated with calloc/realloc (C++ cast).  Count returned in *cnt.  */
static void
sieve_primes(uint64_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out    = 0;
        return;
    }
    uint8_t *is_composite = (uint8_t*)calloc(limit + 1, sizeof *is_composite);
    size_t  cap = limit / 10 + 10;
    uint64_t *primes = (uint64_t*)malloc(cap * sizeof *primes);
    size_t   count = 0;
    for (uint64_t p = 2; p <= limit; ++p) {
        if (!is_composite[p]) {
            if (count == cap) {
                cap <<= 1;
                primes = (uint64_t*)realloc(primes, cap * sizeof *primes);
            }
            primes[count++] = p;
            if (p <= limit / p) {
                for (uint64_t m = p * p; m <= limit; m += p)
                    is_composite[m] = 1;
            }
        }
    }
    free(is_composite);
    *primes_out = primes;
    *cnt_out    = count;
}

/* ------------------------------------------------------------------ */
/* Trial division factorisation of n.  primes[pcnt] and exps[pcnt]
   arrays filled; pcnt returned.  The caller must provide space.      */
static void
factor_uint64(uint64_t n, uint64_t *primes, unsigned *exps, size_t *pcnt)
{
    *pcnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            unsigned e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[(*pcnt)] = p;
            exps[(*pcnt)]   = e;
            ++(*pcnt);
        }
    }
    if (n > 1) {
        primes[(*pcnt)] = n;
        exps[(*pcnt)]   = 1;
        ++(*pcnt);
    }
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Euler's totient for m (≤ 1e4).  Trial division. */
static uint64_t
phi_uint64(uint64_t m)
{
    if (m == 0) return 0;
    uint64_t result = m;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

/* ------------------------------------------------------------------ */
/* Legendre's formula: exponent of prime p in X!.   Returns unsigned. */
static unsigned
factorial_prime_exp(uint64_t X, uint64_t p)
{
    unsigned exp = 0;
    while (X) {
        X /= p;
        exp += (unsigned)X;
    }
    return exp;
}

/* ------------------------------------------------------------------ */
/* Fast exponentiation for seq_t (big int).  Base may be large. */
static seq_t
seq_pow_int(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1ULL) result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1)=1 */

    /* 1. generate primes up to n-1 */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    sieve_primes((uint64_t)n - 1, &primes, &prime_cnt);
    if (prime_cnt == 0) { free(primes); return 1; }

    /* 2. array of exponents for primes, initialised to 0 */
    unsigned *expo = (unsigned*)calloc(prime_cnt, sizeof *expo);

    /* 3. add exponents of (n-1)! */
    uint64_t N = (uint64_t)n - 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        expo[i] = factorial_prime_exp(N, primes[i]);
    }

    /* 4. factorise n and list all its divisors */
    uint64_t pf[20];          /* enough for n≤10k */
    unsigned pe[20];
    size_t pcnt_n = 0;
    factor_uint64((uint64_t)n, pf, pe, &pcnt_n);
    size_t max_div = 1380;    /* upper bound for n≤10k */
    uint64_t *divs = (uint64_t*)malloc(max_div * sizeof *divs);
    size_t div_cnt = 0;
    divisors_from_factor(pf, pe, pcnt_n, divs, &div_cnt);

    /* 5. for each divisor d, subtract exponents of φ(d)! */
    for (size_t idx = 0; idx < div_cnt; ++idx) {
        uint64_t d = divs[idx];
        uint64_t ph = phi_uint64(d);
        if (ph <= 1) {          /* φ(1)=1! =1, no effect */
            continue;
        }
        /* subtract factorial exponents for ph */
        for (size_t i = 0; i < prime_cnt; ++i) {
            if (primes[i] > ph) break;
            expo[i] -= factorial_prime_exp(ph, primes[i]);
        }
    }

    /* 6. reconstruct result from prime exponents */
    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (expo[i] > 0)
            result *= seq_pow_int(primes[i], expo[i]);
    }

    /* 7. free memory */
    free(primes);
    free(expo);
    free(divs);

    return result;
}
