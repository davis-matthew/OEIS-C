/*
 * A094383  Primes p such that d>0 exists and p-d, p-2*d and p-3*d are also
 *          primes.
 * https://oeis.org/A094383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime p such that ∃ d>0 with p-d, p-2d, p-3d all
 *            prime (i.e. p is the largest term of a 4-term prime arithmetic
 *            progression)
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define LIMIT 200000  /* sufficient to contain ≥10000 terms */

static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static bool first_call = true;

static void compute_sequence(void)
{
    /* Sieve of Eratosthenes */
    static char *is_prime = NULL;
    static char *qualify  = NULL;
    static uint32_t *primes = NULL;
    static size_t primes_cap = 0, primes_len = 0;
    size_t i, d, idx;

    /* Allocate arrays only once */
    if (is_prime == NULL) {
        is_prime = (char*)malloc((LIMIT + 1) * sizeof(char));
        qualify  = (char*)malloc((LIMIT + 1) * sizeof(char));
        primes_cap = 20000;
        primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    }

    /* initialize sieve */
    for (i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* collect prime list */
    primes_len = 0;
    for (i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            if (primes_len == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_len++] = (uint32_t)i;
        }
    }

    /* find qualifying primes (largest term of a 4-term AP) */
    for (d = 6; d <= (LIMIT - 2) / 3; d += 6) {
        for (idx = 0; idx < primes_len; ++idx) {
            uint32_t q = primes[idx];
            uint32_t p = q + 3 * d;
            if (p > LIMIT) break;
            if (is_prime[q + d] && is_prime[q + 2 * d] && is_prime[p])
                qualify[p] = 1;
        }
    }

    /* allocate result cache */
    seq_cap = 20000;  /* enough for >10000 terms */
    seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    seq_len = 0;

    for (i = 2; i <= LIMIT; ++i) {
        if (is_prime[i] && qualify[i]) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)i;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, return 0 for safety */
    if (first_call) {
        compute_sequence();
        first_call = false;
    }
    if ((idx_t)seq_len < n) {
        /* n is too large for our precomputed LIMIT,
           increase LIMIT and recompute (not expected for n ≤ 10000) */
        /* fallback: return 0 */
        return (seq_t)0;
    }
    return seq_cache[n - 1];
}
