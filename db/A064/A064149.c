/*
 * A064149  Indices of zeros of third differences of primes: Numbers k such
 *          that A036264(k) = 0.
 * https://oeis.org/A064149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k for which the third difference of primes p_k+3 − 3
 *            p_k+2 + 3 p_k+1 − p_k equals 0. The function computes these k's
 *            by generating primes incrementally and accumulating the third
 *            differences
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static storage for primes and zero‑difference indices */
static uint64_t *prime_vals  = NULL;
static size_t   prime_cap   = 0;
static size_t   prime_len   = 0;

static uint64_t *zero_k_vals = NULL;
static size_t   zero_cap     = 0;
static size_t   zero_len     = 0;

/* ensure prime_vals has capacity at least 'need' */
static void ensure_prime_cap(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_vals, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_vals = tmp;
    prime_cap  = new_cap;
}

/* ensure zero_k_vals has capacity at least 'need' */
static void ensure_zero_cap(size_t need)
{
    if (need <= zero_cap) return;
    size_t new_cap = zero_cap ? zero_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(zero_k_vals, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    zero_k_vals = tmp;
    zero_cap    = new_cap;
}

/* simple trial division primality test using the primes already found */
static int isPrime(uint64_t cand)
{
    if (cand < 2) return 0;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_vals[i];
        if (p * p > cand) break;
        if (cand % p == 0) return 0;
    }
    return 1;
}

/* the required sequence function */
seq_t a(idx_t n)
{
    static uint64_t next_candidate = 2;   /* number to test next for primality */

    /* generate primes and zero‑difference indices until we have at least n */
    while (zero_len < (size_t)n) {
        /* find next prime */
        uint64_t cand = next_candidate;
        while (!isPrime(cand)) cand++;

        /* store the new prime */
        ensure_prime_cap(prime_len + 1);
        prime_vals[prime_len++] = cand;

        /* if we have at least four primes, compute third difference */
        if (prime_len >= 4) {
            long long diff =
                (long long)prime_vals[prime_len - 1]
                - 3LL * (long long)prime_vals[prime_len - 2]
                + 3LL * (long long)prime_vals[prime_len - 3]
                - (long long)prime_vals[prime_len - 4];
            if (diff == 0) {
                /* the zero occurs at index k = prime_len - 3 (1‑based) */
                ensure_zero_cap(zero_len + 1);
                zero_k_vals[zero_len++] = (uint64_t)(prime_len - 3);
            }
        }

        /* advance to next candidate for primality testing */
        next_candidate = cand + 1;
    }

    return (seq_t)zero_k_vals[(size_t)n - 1];
}
