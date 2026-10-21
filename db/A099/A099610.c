/*
 * A099610  a(n) is the smallest odd number that is greater than n^2 and is
 *          the product of two distinct primes.
 * https://oeis.org/A099610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest odd number > n^2 that is the product of two
 *            distinct primes
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/build_primes_upto.h"

static bool primes_ready = false;
static uint64_t *primes = NULL;
static size_t prime_count = 0;

/* Initialise a list of small primes (up to 200 000) once per process. */
static void init_primes(void) {
    if (primes_ready) return;
    const idx_t limit = 200000;
    build_primes_upto(limit, &primes, &prime_count);
    primes_ready = true;
}

/* Test whether a 64‑bit odd integer is a product of two distinct odd primes. */
static bool semiprime_odd_uint64(uint64_t m) {
    if ((m & 1ULL) == 0U) return false;          /* must be odd */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p < 3ULL) continue;                 /* skip 2 */
        uint64_t p2 = p * p;
        if (p2 > m) break;
        if (m % p == 0ULL) {
            uint64_t q = m / p;
            if (q != p && isPrimeGeneric(q))   /* distinct primes */
                return true;
            return false;                      /* square or composite with >2 factors */
        }
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    init_primes();

    /* Compute n² as a seq_t to avoid overflow of idx_t. */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* Find the first odd number greater than n². */
    uint64_t start = (uint64_t)sq;
    if ((start & 1ULL) == 0ULL)
        start += 1ULL;  /* n² even → next odd */
    else
        start += 2ULL;  /* n² odd  → second next odd */

    /* Search forward for the required semiprime. */
    for (;;) {
        if (semiprime_odd_uint64(start))
            return (seq_t)start;
        start += 2ULL;   /* keep the candidate odd */
    }
}
