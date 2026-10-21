/*
 * A051694  Smallest Fibonacci number that is divisible by n-th prime.
 * https://oeis.org/A051694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000045(A001602(n))
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 42,
 *            big any n
 * Helpers:   collect_divisors_rec, fib, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term: the smallest Fibonacci number divisible by the n‑th prime */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/fib.h"
#include "../utils/collect_divisors_rec.h"

/* ----- modular Fibonacci (fast doubling) ----- */
static void fib_mod_pair(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) { *fn = 0; *fn1 = 1; return; }

    uint64_t a, b;
    fib_mod_pair(n >> 1, mod, &a, &b);

    /* c = F(2k) = F(k)*(2*F(k+1) - F(k)) */
    uint64_t two_b = (b << 1) % mod;
    uint64_t two_b_minus_a = (two_b + mod - a) % mod;
    uint64_t c = (uint64_t)((__uint128_t)a * two_b_minus_a % mod);

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
    uint64_t d = (uint64_t)(((__uint128_t)a * a + (__uint128_t)b * b) % mod);

    if ((n & 1) == 0) {      /* even n */
        *fn = c;
        *fn1 = d;
    } else {                  /* odd n */
        *fn = d;
        *fn1 = (c + d) % mod;
    }
}

static uint64_t mod_fib(uint64_t n, uint64_t mod)
{
    uint64_t fn, fn1;
    fib_mod_pair(n, mod, &fn, &fn1);
    return fn;
}

/* ----- prime factorisation of a small integer ----- */
static void factor_uint64(uint64_t n, uint64_t *pr, int *exp, size_t *cnt)
{
    size_t i = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[i] = p;
            exp[i] = e;
            ++i;
        }
    }
    if (n > 1) {
        pr[i] = n;
        exp[i] = 1;
        ++i;
    }
    *cnt = i;
}

/* ----- construct all divisors from factorisation ----- */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    /* 1‑based: get the nth prime */
    uint32_t p = get_prime(n);

    /* Special cases: p = 2 or 5 */
    if (p == 2) return fib((idx_t)3);   /* F(3) = 2  */
    if (p == 5) return fib((idx_t)5);   /* F(5) = 5  */

    /* Bound for the rank of apparition */
    uint64_t bound;
    if (p % 5 == 1 || p % 5 == 4)   /* p ≡ ±1 (mod 5) */
        bound = (uint64_t)p - 1;
    else                            /* otherwise */
        bound = 2ULL * ((uint64_t)p + 1);

    /* Factorise the bound */
    uint64_t prime_factors[32];
    int exponents[32];
    size_t prime_cnt;
    factor_uint64(bound, prime_factors, exponents, &prime_cnt);

    /* Compute number of divisors to allocate array */
    size_t div_max = 1;
    for (size_t i = 0; i < prime_cnt; ++i)
        div_max *= (size_t)(exponents[i] + 1);

    uint64_t *divs = (uint64_t *)malloc(div_max * sizeof(uint64_t));
    size_t div_cnt = 0;
    collect_divisors_rec(0, 1, prime_factors, exponents, prime_cnt, divs, &div_cnt);

    qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

    /* Find the smallest divisor d with p | F(d) */
    uint64_t rank = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        if (d == 0) continue;
        if (mod_fib(d, (uint64_t)p) == 0) {
            rank = d;
            break;
        }
    }
    free(divs);

    /* Fallback (should never hit) */
    if (rank == 0) {
        /* Brute force search up to the bound (unlikely) */
        for (uint64_t d = 1; d <= bound; ++d)
            if (mod_fib(d, (uint64_t)p) == 0) {
                rank = d;
                break;
            }
    }

    return fib((idx_t)rank);
}
