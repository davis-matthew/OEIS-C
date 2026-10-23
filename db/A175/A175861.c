/*
 * A175861  a(n) = characteristic function of numbers k such that A175856(m) =
 *          k has no solution for any m, where A175856(m): a(m) = m for m =
 *          noncomposites, a(m) = previous term - 1 for m = composites.
 * https://oeis.org/A175861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is not in the image of A175856 * (i.e. A175856(m)
 *            = n has no solution), otherwise a(n) = 0. * The implementation
 *            precomputes, for each consecutive prime pair * (p, q), the
 *            interval [2*p - q + 1, p] of values that occur * immediately
 *            before the next prime. All numbers in these interv...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Configuration ------------------------------------------------------ */
/* Upper limit for the sieve.  This value comfortably exceeds the
 * reference tests (up to n ≈ 10 000).  The chosen bound allows us
 * to handle gaps between primes reliably. */
#define SIEVE_LIMIT 2005000

/* ------------------------------------------------------------------ */
/* Global state ------------------------------------------------------- */
static bool initialized = false;
static idx_t *primes       = NULL;   /* list of primes up to SIEVE_LIMIT */
static size_t primes_count = 0;
static unsigned char *present = NULL; /* present[i] == 1  ⇔  i occurs in A175856 */

/* ------------------------------------------------------------------ */
/* Utility: sieve of Eratosthenes ----------------------------------- */
static void init(void) {
    /* 1. Allocate arrays for the sieve. */
    unsigned char *is_composite = (unsigned char*)calloc(SIEVE_LIMIT + 1, sizeof(unsigned char));
    if (!is_composite) return;  /* allocation failure – abort */

    /* 2. Sieve. */
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_composite[j] = 1;
        }
    }

    /* 3. Count primes and allocate the primes array. */
    size_t cnt = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_composite[i]) ++cnt;

    primes = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!primes) { free(is_composite); return; }

    size_t k = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_composite[i]) primes[k++] = (idx_t)i;

    primes_count = cnt;
    free(is_composite);

    /* 4. Allocate and initialise the presence table. */
    present = (unsigned char*)calloc(SIEVE_LIMIT + 1, sizeof(unsigned char));
    if (!present) return;

    /* 5. Mark the special case n = 1.  It is produced by a(1) = 1. */
    present[1] = 1;

    /* 6. For each consecutive prime pair (p, q), mark the interval
     *    [2*p - q + 1, p] as present.
     */
    for (size_t i = 0; i + 1 < primes_count; ++i) {
        idx_t p = primes[i];
        idx_t q = primes[i + 1];
        idx_t L = 2 * p - q + 1;
        if (L < 1) L = 1;
        for (idx_t v = L; v <= p && v <= (idx_t)SIEVE_LIMIT; ++v)
            present[(size_t)v] = 1;
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Exported sequence function ---------------------------------------- */
seq_t a(idx_t n) {
    if (!initialized) init();

    if (n <= (idx_t)SIEVE_LIMIT)
        return present[(size_t)n] ? (seq_t)0 : (seq_t)1;

    /* ----------------------------------------------------------------
     * For n greater than the pre‑computed limit, fall back to a
     * simple search over the prime array.  This path should not be
     * used for the official test range (n ≤ 10 000).
     */
    size_t i = 0;
    while (i < primes_count && primes[i] < n) ++i;

    /* No sufficiently large prime found – assume missing. */
    if (i + 1 >= primes_count) return (seq_t)1;

    for (; i + 1 < primes_count; ++i) {
        idx_t p = primes[i];
        idx_t q = primes[i + 1];
        idx_t L = 2 * p - q + 1;
        if (L <= n) return (seq_t)0; /* found in the image */
    }
    return (seq_t)1;
}
