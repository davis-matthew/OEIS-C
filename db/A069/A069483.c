/*
 * A069483  Largest prime factor of prime(n+1)^2 - prime(n)^2.
 * https://oeis.org/A069483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of prime(n+1)^2 – prime(n)^2 a(1) =
 *            5, a(2) = 2, ... (OEIS A069483)
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Maximal prime needed for n up to about 10000.
   10000th prime is 104729, so primes up to 200000 are sufficient. */
#define PRIME_LIMIT 200000

static bool primes_built = false;
static unsigned prime_cnt = 0;
static int prime_arr[PRIME_LIMIT + 1];

/* Simple sieve of Eratosthenes to fill prime_arr. */
static void build_primes(void) {
    if (primes_built) return;
    bool is_prime[PRIME_LIMIT + 1];
    for (int i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (is_prime[p]) {
            for (int k = p * p; k <= PRIME_LIMIT; k += p) is_prime[k] = false;
        }
    }
    unsigned idx = 0;
    for (int n = 2; n <= PRIME_LIMIT; ++n) {
        if (is_prime[n]) {
            prime_arr[idx++] = n;
        }
    }
    prime_cnt = idx;
    primes_built = true;
}

/* Return the n-th prime (1‑based). Assumes n>=1 and n<=prime_cnt. */
static int get_prime(unsigned n) {
    return prime_arr[n - 1];
}

/* Largest prime factor of a 64‑bit value. Uses the prime list. */
static seq_t lpf_uint64(uint64_t m) {
    seq_t largest = 1;
    for (unsigned i = 0; i < prime_cnt; ++i) {
        int p = prime_arr[i];
        if ((uint64_t)p * (uint64_t)p > m) break;
        while (m % p == 0) {
            largest = (seq_t)p;
            m /= p;
        }
    }
    if (m > 1) largest = (seq_t)m;
    return largest;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Ensure primes are available */
    if (!primes_built) build_primes();

    /* guard: n >= 1 */
    if (n < 1) return 0;  /* undefined, but return 0 */

    /* fetch consecutive primes */
    int p1 = get_prime((unsigned)n);   /* prime(n)   */
    int p2 = get_prime((unsigned)n + 1); /* prime(n+1) */

    /* compute (p2^2 - p1^2) = (p2 - p1) * (p2 + p1) */
    uint64_t diff = (uint64_t)p2 - (uint64_t)p1;
    uint64_t sum  = (uint64_t)p2 + (uint64_t)p1;
    uint64_t prod = diff * sum;        /* fits in 64 bits */

    return lpf_uint64(prod);
}
