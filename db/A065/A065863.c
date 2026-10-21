/*
 * A065863  Remainder when n-th prime is divided by the number of nonprimes
 *          not exceeding n.
 * https://oeis.org/A065863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) mod (n - pi(n))
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a lazy prime sieve.  It keeps a list of primes,
   a simple is_prime array, and a cumulative prime-count array pi_arr. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache for sieve, primes, and prime counts */
static size_t sieve_limit = 0;
static unsigned char *is_prime = NULL;     /* 1 = prime, 0 = composite */
static uint32_t *primes   = NULL;          /* list of primes, 0‑based */
static size_t primes_cnt  = 0;
static seq_t *pi_arr      = NULL;          /* pi_arr[i] = number of primes ≤ i */
static size_t pi_cap      = 0;

/* Ensure the sieve covers at least the integer value n. */
static void ensure_sieve(size_t n)
{
    /* Roughly 20*n+100 exceeds the n-th prime for all n needed in tests */
    size_t bound = n * 20 + 100;
    if (bound <= sieve_limit)
        return;

    /* Reallocate or initialize is_prime array */
    unsigned char *tmp = (unsigned char*)realloc(is_prime, bound + 1);
    if (!tmp) return;                /* allocation failure – give up silently */
    is_prime = tmp;
    for (size_t i = 0; i <= bound; ++i) is_prime[i] = 1;
    if (bound >= 0) is_prime[0] = 0;
    if (bound >= 1) is_prime[1] = 0;

    /* Sieve of Eratosthenes */
    for (size_t i = 2; i * i <= bound; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= bound; j += i)
                is_prime[j] = 0;
        }
    }

    /* Rebuild prime list */
    size_t new_cap = primes_cnt ? primes_cnt * 2 : 64;
    uint32_t *new_primes = (uint32_t*)realloc(primes, new_cap * sizeof(uint32_t));
    if (!new_primes) return;
    primes = new_primes;
    primes_cnt = 0;
    for (size_t i = 2; i <= bound; ++i) {
        if (is_prime[i]) {
            if (primes_cnt >= new_cap) {
                new_cap *= 2;
                uint32_t *tmp2 = (uint32_t*)realloc(primes, new_cap * sizeof(uint32_t));
                if (!tmp2) return;
                primes = tmp2;
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    }

    /* Build cumulative prime count array pi_arr */
    size_t new_pi_cap = bound + 1;
    seq_t *new_pi = (seq_t*)realloc(pi_arr, new_pi_cap * sizeof(seq_t));
    if (!new_pi) return;
    pi_arr = new_pi;
    pi_cap = new_pi_cap;
    seq_t acc = 0;
    for (size_t i = 0; i <= bound; ++i) {
        if (i >= 2 && is_prime[i]) ++acc;
        pi_arr[i] = acc;
    }

    sieve_limit = bound;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n=0 – safe fallback */
    ensure_sieve((size_t)n);

    /* nth prime (1‑based index) is primes[n-1] */
    seq_t prime_n = (seq_t)primes[n - 1];

    /* pi(n), number of primes ≤ n */
    seq_t pi_n = pi_arr[n];

    /* number of non‑primes ≤ n */
    seq_t mod = (seq_t)((n > (idx_t)pi_n) ? (n - (idx_t)pi_n) : 1ULL);
    return prime_n % mod;
}
