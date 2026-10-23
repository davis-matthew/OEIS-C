/*
 * A106278  Number of distinct zeros of x^5-x^4-x^3-x^2-x-1 mod prime(n).
 * https://oeis.org/A106278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A106278: number of distinct zeros of x^5 - x^4 - x^3 - x^2 - x - 1 (mod prime(n))
   a(n) = |{ x ∈ [0, prime(n)−1] : x^5 - x^4 - x^3 - x^2 - x - 1 ≡ 0 (mod prime(n)) }|   */
#include <stdint.h>
#include <stdbool.h>

#define SIEVE_LIMIT 200000          /* contains at least 20000 primes, enough for n up to 10000 */
#define MAX_PRIME_CNT 20000        /* buffer size for primes */

static bool sieve_done = false;
static unsigned int primes[MAX_PRIME_CNT];
static unsigned int primes_cnt = 0;

/* simple sieve of Eratosthenes up to SIEVE_LIMIT */
static void init_sieve(void)
{
    if (sieve_done) return;
    bool is_composite[SIEVE_LIMIT + 1] = { false };
    for (unsigned int i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (unsigned int j = i * i; j <= SIEVE_LIMIT; j += i)
                is_composite[j] = true;
        }
    }
    for (unsigned int i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (primes_cnt < MAX_PRIME_CNT) primes[primes_cnt++] = i;
        }
    }
    sieve_done = true;
}

/* return the n-th prime, 1‑based; returns 0 if n is out of range */
static unsigned int get_prime(idx_t n)
{
    init_sieve();
    if (n == 0) return 0;
    if (n <= (idx_t)primes_cnt) return primes[(unsigned int)n - 1];
    return 0;   /* out of precomputed range – should not happen for n ≤ 10000 */
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined offset    */
    unsigned int p = get_prime(n);
    if (p == 0) return 0;                      /* safety return       */

    unsigned int count = 0;
    uint64_t mod = p;                           /* local mod for speed */

    for (unsigned int x = 0; x < p; ++x) {
        uint64_t t = x;                         /* x modulo p           */
        uint64_t x2 = (t * t) % mod;
        uint64_t x3 = (x2 * t) % mod;
        uint64_t x4 = (x3 * t) % mod;
        uint64_t x5 = (x4 * t) % mod;

        uint64_t sum = (x4 + x3 + x2 + t + 1) % mod;   /* x^4 + x^3 + x^2 + x + 1 */
        uint64_t fval = (x5 + mod - sum) % mod;         /* f(x) modulo p       */

        if (fval == 0) ++count;
    }
    return (seq_t)count;                        /* count ≤ 5, fits in all seq_t sizes */
}
