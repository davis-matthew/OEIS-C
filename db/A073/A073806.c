/*
 * A073806  Number of divisors of sum of square of divisors.
 * https://oeis.org/A073806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of Σ_{d|n} d² (σ₂(n))
 * Range:     u8 n <= 1819, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma2_from_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute σ₂(n) by factorisation of n, then compute the number of
    divisors of that σ₂ value.  All arithmetic is performed in 64‑bit
    unsigned type; the final result is cast to seq_t.  A static prime
    list (up to 100 000) is built once and reused for all calls.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/sigma2_from_factors.h"

static uint64_t *prime_list = NULL;   /* primes ≤ 100 000 */
static size_t   prime_cnt   = 0;
static bool     primes_init = false;

/* Initialise the prime list (once).  */
static void init_primes(void)
{
    if (primes_init) return;
    const size_t limit = 100000;               /* > sqrt(max σ₂ for n≈10⁴) */
    size_t      size   = limit + 1;
    char        *is_comp = (char*)calloc(size, 1);
    size_t      cap     = limit / 10 + 10;

    prime_list = (uint64_t*)malloc(cap * sizeof(uint64_t));

    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt == cap) {
                cap *= 2;
                prime_list = (uint64_t*)realloc(prime_list, cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    primes_init = true;
}

/* Factorise val into primes and exponents (stored in arrays up to
   a maximum of 32 factors).  The function returns the number of
   prime factors found.  */
static size_t factorise(uint64_t val,
                       uint64_t *pfactors, uint64_t *pexps)
{
    size_t pc = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= val; ++i) {
        uint64_t p = prime_list[i];
        if (val % p == 0) {
            uint64_t e = 0;
            while (val % p == 0) {
                val /= p;
                ++e;
            }
            pfactors[pc] = p;
            pexps[pc++]  = e;
        }
    }
    if (val > 1) {
        pfactors[pc] = val;
        pexps[pc++]  = 1;
    }
    return pc;
}

/* Return the number of divisors of x. */
static uint64_t divisor_count(uint64_t x)
{
    if (x <= 1) return 1;
    uint64_t factors[32], exps[32];
    size_t pc = factorise(x, factors, exps);
    uint64_t cnt = 1;
    for (size_t i = 0; i < pc; ++i)
        cnt *= (exps[i] + 1);
    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* initialisation of primes list (once) */
    init_primes();

    /* trivial case n = 1 */
    if (n <= 1)
        return (seq_t)1;

    /* factorise n */
    uint64_t pf[32], pe[32];
    size_t pc = factorise((uint64_t)n, pf, pe);

    /* compute σ₂(n) */
    uint64_t sigma2 = sigma2_from_factors(pf, pe, pc);

    /* compute d(σ₂(n)) */
    uint64_t d = divisor_count(sigma2);

    return (seq_t)d;
}
