/*
 * A078926  Number of primitive Pythagorean triangles with perimeter 2n.
 * https://oeis.org/A078926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # of odd unitary divisors d of n with sqrt(n) < d <
 *            sqrt(2n) (which equals the number of primitive Pythagorean
 *            triangles with perimeter 2n)
 * Range:     u8 n <= 9705, u16 n <= 9705, u32 n <= 9705, u64 n <= 9705,
 *            u128 n <= 9705, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t idx_t;

/* We will cache prime numbers up to the required upper bound.
   Primes are stored in static arrays that grow as needed. */
static uint32_t *primes = NULL;          /* prime list */
static size_t  prime_count = 0;          /* number of stored primes */
static idx_t   prime_limit = 0;          /* current largest sieved value */

/* Prime sieve: build all primes <= limit. */
static void init_primes(idx_t limit) {
    if (limit <= prime_limit) return;   /* already have enough primes */
    if (limit < 2) limit = 2;

    /* Sieve of Eratosthenes using a bitset */
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    memset(is_prime, 1, limit + 1);
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Count primes and copy them */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    primes = (uint32_t *)realloc(primes, cnt * sizeof(uint32_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[pos++] = (uint32_t)i;
    prime_count = cnt;
    prime_limit = limit;

    free(is_prime);
}

/* Factor n into prime powers. Result stored in the static arrays passed.
   The caller must provide space for at least 64 factors. */
static void factor_uint64(idx_t n, uint32_t *pf, uint8_t *exp, size_t *cnt) {
    *cnt = 0;
    if (n == 0) return;                /* not expected */
    init_primes((idx_t)((double) n * 0.5 + 2)); /* ensure enough primes for sqrt(n) */

    for (size_t i = 0; i < prime_count && (idx_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if (n % p == 0) {
            uint8_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pf[*cnt] = p;
            exp[*cnt] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {                       /* remaining prime factor */
        pf[*cnt] = (uint32_t)n;
        exp[*cnt] = 1;
        ++(*cnt);
    }
}

/* The main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0; /* undefined, return 0 */

    /* factor n */
    uint32_t pf[64];
    uint8_t  exp[64];
    size_t   cnt;
    factor_uint64(n, pf, exp, &cnt);

    /* collect odd prime powers (unitary divisor cannot include factor 2) */
    uint64_t odd_pow[64];
    size_t   odd_cnt = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (pf[i] == 2) continue;
        uint64_t val = 1;
        for (uint8_t e = 0; e < exp[i]; ++e) val *= (uint64_t)pf[i];
        odd_pow[odd_cnt++] = val;
    }

    __int128 two_n = (__int128)n * 2;
    unsigned long long result = 0;
    size_t subsets = 1U << odd_cnt; /* 2^odd_cnt */
    for (size_t mask = 0; mask < subsets; ++mask) {
        __int128 d = 1;
        for (size_t i = 0; i < odd_cnt; ++i)
            if (mask & (1U << i))
                d *= odd_pow[i];
        __int128 d2 = d * d;
        if (d2 > (__int128)n && d2 < two_n)
            ++result;
    }
    return result;
}
