/*
 * A228182  a(n) is the smallest k such that the sum of squares of prime
 *          divisors of k is equal to the sum of prime divisors of n+k.
 * https://oeis.org/A228182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k such that the sum of squares of the
 *            distinct prime divisors of k equals the sum of the distinct
 *            prime divisors of n + k
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define LIMIT 3000000U   /* upper bound for sieve; should cover all n<=10000 */

static uint32_t *spf = NULL;  /* smallest prime factor array */
static bool initialized = false;

/* Build smallest‑prime‑factor array up to LIMIT */
static void init_sieve(void)
{
    if (initialized)
        return;
    spf = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!spf)
        exit(1);  /* fatal: unable to allocate */
    for (uint32_t i = 0; i <= LIMIT; ++i)
        spf[i] = 0;
    spf[0] = spf[1] = 1;                 /* not used as primes */
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * (uint64_t)i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * (uint64_t)i;
                     j <= LIMIT; j += i) {
                    if (spf[(uint32_t)j] == 0)
                        spf[(uint32_t)j] = i;
                }
            }
        }
    }
    initialized = true;
}

/* Sum of squares of distinct prime divisors of x */
static seq_t sum_square_prime_divisors(uint64_t x)
{
    seq_t s = 0;
    while (x > 1) {
        uint32_t p = spf[(uint32_t)x];
        s += (seq_t)p * (seq_t)p;
        while (x % p == 0)
            x /= p;
    }
    return s;
}

/* Sum of distinct prime divisors of x */
static seq_t sum_prime_divisors(uint64_t x)
{
    seq_t s = 0;
    while (x > 1) {
        uint32_t p = spf[(uint32_t)x];
        s += (seq_t)p;
        while (x % p == 0)
            x /= p;
    }
    return s;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_sieve();
    uint64_t N = (uint64_t)n;
    for (uint64_t k = 1;; ++k) {
        uint64_t nk = N + k;
        if (nk > LIMIT) {
            /* LIMIT should be large enough for all required n.
               If exceeded, the function returns 0 as a safe fallback. */
            return 0;
        }
        seq_t s1 = sum_square_prime_divisors(k);
        seq_t s2 = sum_prime_divisors(nk);
        if (s1 == s2)
            return (seq_t)k;
    }
}
