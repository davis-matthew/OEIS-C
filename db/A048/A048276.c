/*
 * A048276  a(n) = number of squarefree numbers among C(n,k), k=0..n.
 * https://oeis.org/A048276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k (0 ≤ k ≤ n) such that C(n,k) is squarefree
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation based on prime factorisation of factorials:
   C(n,k) is squarefree iff for every prime p the exponent
   v_p(n!) – v_p(k!) – v_p((n-k)!) is ≤ 1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *primes = NULL;                 /* list of primes ≤ max_n   */
static uint16_t **fac = NULL;                  /* fac[pi][m] = v_p(m!)     */
static size_t prime_count = 0;                 /* number of primes cached  */
static idx_t max_n = 0;                        /* largest n cached         */

/* Helper: sieve to generate primes ≤ limit */
static void build_primes_limit(idx_t limit) {
    if (limit < 2) return;
    size_t sz = limit + 1;
    bool *is_prime = (bool*)malloc(sz);
    for (size_t i = 0; i < sz; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }
    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;
    /* Allocate and fill primes array */
    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[pos++] = (uint32_t)i;
    prime_count = cnt;
    free(is_prime);
}

/* Helper: build factorial prime exponent arrays up to n */
static void build_factorial_arrays(idx_t n) {
    if (n <= max_n) return;          /* already built up to ≥ n */
    /* First ensure primes up to n are available */
    build_primes_limit(n);
    /* Allocate exponent arrays for each prime */
    fac = (uint16_t**)realloc(fac, prime_count * sizeof(uint16_t*));
    for (size_t pi = 0; pi < prime_count; ++pi) {
        fac[pi] = (uint16_t*)calloc((size_t)(n + 1), sizeof(uint16_t));
        uint32_t p = primes[pi];
        /* Compute v_p(m!) for m = 1..n */
        for (idx_t m = 1; m <= n; ++m) {
            int cnt = 0;
            int temp = (int)m;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            fac[pi][m] = fac[pi][m - 1] + (uint16_t)cnt;
        }
    }
    max_n = n;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Special trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    build_factorial_arrays(n);

    seq_t count = 1;           /* k = 0 is always squarefree */

    for (idx_t k = 1; k <= n; ++k) {
        bool squarefree = true;
        for (size_t pi = 0; pi < prime_count; ++pi) {
            uint16_t e = fac[pi][n] - fac[pi][k] - fac[pi][n - k];
            if (e > 1) {          /* exponent too large, not squarefree */
                squarefree = false;
                break;
            }
        }
        if (squarefree) ++count;
    }
    return count;
}
