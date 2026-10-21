/*
 * A056670  Largest non-unitary prime factor of A001405(n) = binomial(n,
 *          floor(n/2)), or 1 if no such prime exists.
 * https://oeis.org/A056670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime p such that the exponent of p in
 *            binomial(n, ⌊n/2⌋) is at least 2; 1 if no such prime exists
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ----- Legendre's formula: exponent of p in m! ----- */
static inline uint32_t exp_in_factorial(uint64_t m, uint64_t p)
{
    uint64_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return (uint32_t)e;
}

/* ----- Small sieve.  primes[] holds the primes up to the largest limit
   requested so far.  On each call with a larger limit, the array is
   rebuilt (no reuse of partial data).  ---- */
static uint32_t *primes = NULL;
static size_t   primes_cnt = 0, primes_cap = 0;
static uint64_t primes_max = 0;

static void build_primes_upto(uint64_t limit)
{
    if (limit <= primes_max) return;          /* already sufficient */

    size_t sz = limit + 1;
    uint8_t *is_prime = (uint8_t *)malloc(sz);
    if (!is_prime) return;                    /* allocation failure */

    /* initialise everything as prime, then wipe 0 and 1 */
    for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* free old prime list */
    free(primes);
    primes = NULL;
    primes_cnt = 0;
    primes_cap = 0;

    /* rebuild prime list */
    for (uint64_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *)realloc(primes,
                                                    primes_cap * sizeof(uint32_t));
                if (!tmp) { free(is_prime); return; }
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    }

    primes_max = limit;
    free(is_prime);
}

/* ----- Sequence function ----- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;        /* offset starts at 1 */

    uint64_t kk = n / 2;                /* floor(n/2) */
    uint64_t mm = n - kk;               /* n - kk (used for odd n) */
    build_primes_upto(n);               /* need all primes ≤ n */

    uint32_t largest = 0;               /* prime with exponent ≥ 2 */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p > n) break;     /* primes > n cannot contribute */

        uint32_t e_n = exp_in_factorial(n,    p);
        uint32_t e_k = exp_in_factorial(kk,  p);
        uint32_t e_m = exp_in_factorial(mm,  p);
        uint32_t e_p = e_n - e_k - e_m;      /* exponent of p in C(n,kk) */

        if (e_p >= 2 && p > largest)
            largest = p;
    }

    return largest ? (seq_t)largest : (seq_t)1;
}
