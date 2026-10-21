/*
 * A068111  Numerators of coefficients in J0(i*sqrt(x))^2, where J0 denotes
 *          the ordinary Bessel function of order 0.
 * https://oeis.org/A068111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = ∏{p | n < p ≤ 2n, p odd prime} (empty product = 1)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 26, u64 n <= 50, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- prime sieve infrastructure ---------------------------------------- */

static bool *sieve_arr      = NULL;   /* true = composite */
static size_t sieve_limit   = 0;      /* largest number sieved (≥ 2)   */
static uint64_t *prime_arr   = NULL;   /* list of primes (including 2)  */
static size_t prime_count   = 0;
static size_t prime_cap     = 0;

/* rebuild sieve and prime list up to at least 'limit' (limit ≥ 2) */
static void rebuild_sieve(size_t limit) {
    /* allocate/shrink the composite array */
    if (sieve_limit < limit) {
        sieve_arr = (bool*)realloc(sieve_arr, (limit + 1) * sizeof(bool));
        if (!sieve_arr) abort();           /* out of memory */
        for (size_t i = 0; i <= limit; ++i) sieve_arr[i] = false;
    }

    /* simple sieve of Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!sieve_arr[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve_arr[j] = true;
        }
    }

    /* count primes up to 'limit' */
    size_t count = 0;
    if (limit >= 2) count++;               /* prime 2 */

    for (size_t i = 3; i <= limit; i += 2) {
        if (!sieve_arr[i]) count++;
    }

    /* grow the prime array if necessary */
    if (prime_cap < count) {
        prime_arr = (uint64_t*)realloc(prime_arr, count * sizeof(uint64_t));
        if (!prime_arr) abort();
        prime_cap = count;
    }

    /* fill the prime list */
    size_t idx = 0;
    if (limit >= 2) prime_arr[idx++] = 2;
    for (size_t i = 3; i <= limit; i += 2) {
        if (!sieve_arr[i]) prime_arr[idx++] = i;
    }
    prime_count = idx;
    sieve_limit = limit;
}

/* ---------------------------------------------------------------------- */

seq_t a(idx_t n) {
    /* base case */
    if (n == 0) return (seq_t)1;

    /* 2*n is the largest prime we need to consider */
    size_t needed = 2 * (size_t)n;
    if (needed < 2) needed = 2;

    /* ensure primes up to needed are available */
    if (sieve_limit < needed)
        rebuild_sieve(needed);

    seq_t prod = (seq_t)1;

    /* multiply odd primes greater than n and ≤ 2n */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_arr[i];
        if (p <= (uint64_t)n) continue;
        if (p > (uint64_t)needed) break;
        if ((p & 1ULL) == 0) continue;   /* skip even prime 2 */
        prod *= (seq_t)p;
    }

    return prod;
}
