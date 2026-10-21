/*
 * A075526  a(n) = A008578(n+2) - A008578(n+1).
 * https://oeis.org/A075526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+1) – prime(n) for n ≥ 1 (a(0) = 1)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  prime(k) is the k‑th prime: 2,3,5,7,11,...  */
/*  Sequence is the list of primes gaps. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Maximum n we expect to handle. */
#define MAX_N 10000
/* Number of primes needed: we need up to prime(MAX_N+1). */
#define PRIMES_NEEDED (MAX_N + 1)

/* Upper bound on the sieve. 200 000 guarantees > 18 000 primes. */
#define SIEVE_LIMIT 200000

/* Static storage for primes. 8 bytes * 200 000 ≈ 1.6 MB. */
static uint64_t primes[SIEVE_LIMIT];
static size_t primes_count = 0;

/* Sieve only once. */
static void init_primes(void)
{
    static bool initialized = false;
    if (initialized) return;

    /* Sieve of Eratosthenes.  We keep an auxiliary boolean array */
    static bool is_composite[SIEVE_LIMIT + 1];
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[primes_count++] = i;
            if (i * i <= SIEVE_LIMIT) {
                for (size_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                    is_composite[j] = true;
                }
            }
        }
    }
    initialized = true;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 by definition. */

    init_primes();                   /* ensures primes[] is filled. */

    /* n ≥ 1 so primes[n] and primes[n-1] exist for n ≤ MAX_N. */
    /* For safety, we tolerate n beyond our precomputed range by extending
       the sieve region, but this is not expected in the test harness. */
    if ((size_t)n >= primes_count) {
        /* Fallback: compute next primes naively (slow, but harmless for rare cases). */
        uint64_t val = primes[primes_count - 1] + 1;
        uint64_t prev = primes[primes_count - 1];
        size_t needed = (size_t)n + 1;
        while (primes_count <= needed) {
            /* Find next prime. */
            bool is_prime = true;
            for (size_t p = 0; p < primes_count; ++p) {
                uint64_t q = primes[p];
                if (q * q > val) break;
                if (val % q == 0) { is_prime = false; break; }
            }
            if (is_prime) {
                primes[primes_count++] = val;
            }
            val += 1;
        }
    }

    /* Return the prime gap. */
    return (seq_t)(primes[(size_t)n] - primes[(size_t)n - 1]);
}
