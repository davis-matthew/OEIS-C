/*
 * A058977  For a rational number p/q let f(p/q) = sum of distinct prime
 *          factors (A008472) of p+q divided by number of distinct prime
 *          factors (A001221) of p+q; a(n) is obtained by iterating f,
 *          starting at n/1, until an integer is reached, or if no integer is
 *          ever reached then a(n) = 0.
 * https://oeis.org/A058977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ugcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is obtained by iterating f starting at n/1 until an integer is reached. 
 * f(p/q) = S(p+q) / Ω(p+q), where S is the sum of distinct prime factors and Ω is the number of distinct prime factors of p+q. 
 * The fraction is reduced by its gcd at each step; a(n) = the integer reached (when the denominator becomes 1), otherwise a(n)=0. 
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ugcd_uint64.h"

#define PRIME_MAX 200000U   /* sufficient for n up to ~10⁴ and intermediate sums */

/* Prime sieve table */
static uint32_t primes[PRIME_MAX];
static size_t prime_cnt = 0;
static int primes_ready = 0;

/* Build a static list of primes up to PRIME_MAX */
static void build_primes(void)
{
    if (primes_ready) return;
    for (size_t i = 0; i < PRIME_MAX; ++i) primes[i] = (uint32_t)i;
    prime_cnt = 0;
    for (uint32_t p = 2; (uint64_t)p * p < PRIME_MAX; ++p)
        if (primes[p] == p)
            for (uint32_t j = p * p; j < PRIME_MAX; j += p)
                if (primes[j] == j) primes[j] = p;
    for (uint32_t i = 2; i < PRIME_MAX; ++i)
        if (primes[i] == i) primes[prime_cnt++] = i;
    primes_ready = 1;
}

/* Compute sum of distinct prime factors and count of distinct prime factors of n */
static void factor_sum_cnt(uint64_t n, uint64_t *sum, uint64_t *cnt)
{
    *sum = 0;
    *cnt = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            *sum += p;
            ++(*cnt);
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {
        *sum += temp;
        ++(*cnt);
    }
}

seq_t a(idx_t n)
{
    build_primes();                     /* ensure primes are ready */

    uint64_t p = (uint64_t)n;          /* numerator */
    uint64_t q = 1ULL;                 /* denominator */

    for (int step = 0; step < 1000; ++step) {
        uint64_t s, c;
        factor_sum_cnt(p + q, &s, &c);

        /* Reduce the fraction s/c */
        uint64_t g = ugcd_uint64(s, c);
        if (g > 1) {
            s /= g;
            c /= g;
        }

        if (c == 1)                    /* integer reached */
            return (seq_t)s;

        /* Prepare next iteration */
        p = s;
        q = c;
    }
    return (seq_t)0;                     /* did not converge within step limit */
}
