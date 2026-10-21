/*
 * A083901  Number of divisors of n with largest digit <= 6 (base 10).
 * https://oeis.org/A083901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors d of n such that all decimal digits of
 *            d are ≤ 6
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   good_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/good_divisor.h"

/* ----- Prime sieve ----- */
static int *primes = NULL;          /* array of primes */
static size_t prime_cnt = 0;        /* number of primes */
static int sieve_limit = 1000000;   /* sieve up to 1,000,000 */

/* Simple sieve of Eratosthenes */
static void init_primes(void)
{
    if (primes) return; /* already initialized */
    primes = (int *)malloc((sieve_limit + 1) * sizeof(int));
    for (int i = 2; i <= sieve_limit; ++i) primes[i] = 1;
    for (int i = 2; i * i <= sieve_limit; ++i) {
        if (primes[i]) {
            for (int j = i * i; j <= sieve_limit; j += i)
                primes[j] = 0;
        }
    }
    /* count primes */
    prime_cnt = 0;
    for (int i = 2; i <= sieve_limit; ++i) if (primes[i]) ++prime_cnt;
    /* compact array */
    int *tmp = (int *)malloc(prime_cnt * sizeof(int));
    size_t p = 0;
    for (int i = 2; i <= sieve_limit; ++i)
        if (primes[i]) tmp[p++] = i;
    free(primes);
    primes = tmp;
}

/* Recursive generation of divisors */
static void gen_divs(const uint64_t *pr, const int *ex, size_t pcnt,
                     size_t idx, uint64_t cur, int *count)
{
    if (idx == pcnt) {
        if (good_divisor(cur)) ++(*count);
        return;
    }
    uint64_t p = pr[idx];
    uint64_t v = 1;
    for (int e = 0; e <= ex[idx]; ++e) {
        gen_divs(pr, ex, pcnt, idx + 1, cur * v, count);
        v *= p;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure primes are ready */
    if (!primes) init_primes();

    if (n == 0) return 0; /* undefined case, but return 0 */

    /* Factorization of n */
    uint64_t m = (uint64_t)n;
    uint64_t pr[64];
    int ex[64];
    size_t pcnt = 0;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= m; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            pr[pcnt] = p;
            ex[pcnt] = e;
            ++pcnt;
        }
    }
    if (m > 1) {          /* remaining prime factor */
        pr[pcnt] = m;
        ex[pcnt] = 1;
        ++pcnt;
    }

    int count = 0;
    gen_divs(pr, ex, pcnt, 0, 1ULL, &count);
    return (seq_t)count;
}
