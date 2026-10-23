/*
 * A100742  Smallest prime p such that (p+prime(n))/2 is prime.
 * https://oeis.org/A100742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = smallest prime p such that (p + prime(n))/2 is prime
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/isPrime64.h"   /* bool isPrime64(uint64_t n); */

static uint64_t *global_primes = NULL;   /* list of primes (as uint64_t) */
static size_t      global_prime_count = 0;
static uint64_t    sieve_limit = 0;

/* Simple sieve up to 'limit' and rebuild the global prime list. */
static void recompute_primes(uint64_t limit)
{
    if (limit <= sieve_limit) return;

    /* Allocate sieve bitmap */
    uint8_t *sieve = (uint8_t *)malloc((size_t)(limit + 1));
    if (!sieve) exit(EXIT_FAILURE);           /* out of memory */
    /* set all to true (prime) */
    for (uint64_t i = 0; i <= limit; ++i) sieve[i] = 1;
    if (limit >= 0) sieve[0] = sieve[1] = 0;   /* 0,1 not prime */

    /* Eratosthenes */
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;
        }
    }

    /* Rebuild prime list */
    free(global_primes);
    global_primes = NULL;
    global_prime_count = 0;
    size_t capacity = 32;
    global_primes = (uint64_t *)malloc(capacity * sizeof(uint64_t));
    if (!global_primes) exit(EXIT_FAILURE);

    for (uint64_t i = 2; i <= limit; ++i) {
        if (sieve[i]) {
            if (global_prime_count == capacity) {
                capacity <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(global_primes, capacity * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                global_primes = tmp;
            }
            global_primes[global_prime_count++] = i;
        }
    }

    free(sieve);
    sieve_limit = limit;
}

/* Helper: ensure we have at least n primes in the global list. */
static void ensure_n_primes(size_t n)
{
    if (global_prime_count >= n) return;

    /* Quick upper bound for the n-th prime: n*(20)+100 (enough for n<=10^6). */
    uint64_t limit = (n < 6) ? 15 : n * 20 + 100;

    /* Grow until we have enough primes. */
    while (global_prime_count < n) {
        recompute_primes(limit);
        if (global_prime_count >= n) break;
        limit *= 2;          /* double if still insufficient */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined but safe */

    /* 1‑based index */
    size_t idx = (size_t)n;

    /* Ensure prime(n) is known */
    ensure_n_primes(idx);
    uint64_t prime_n = global_primes[idx - 1];   /* nth prime (1‑based) */

    /* Search for the smallest prime p satisfying the condition. */
    /* First check primes already computed. */
    for (size_t i = 0; i < global_prime_count; ++i) {
        uint64_t p = global_primes[i];
        uint64_t sum = p + prime_n;
        if (sum & 1) continue;            /* sum must be even */
        uint64_t q = sum >> 1;            /* (p + prime_n)/2 */
        if (isPrime64(q)) return (seq_t)p;
    }

    /* If not found, continue testing odd candidates beyond current sieve. */
    uint64_t candidate = (global_primes[global_prime_count - 1] | 1) + 2; /* next odd after last prime */
    for (;;) {
        if (isPrime64(candidate)) {
            uint64_t sum = candidate + prime_n;
            if ((sum & 1) == 0) {
                uint64_t q = sum >> 1;
                if (isPrime64(q))
                    return (seq_t)candidate;
            }
        }
        candidate += 2;  /* skip even numbers */
    }
}
