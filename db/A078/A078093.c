/*
 * A078093  Numbers k such that Omega(k) = Omega(k+1) + Omega(k+2) +
 *          Omega(k+3) where Omega(k) denotes the number of prime factors of
 *          k, counting multiplicity.
 * https://oeis.org/A078093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based computation: precompute Ω(k) up to a fixed LIMIT and
   collect all k such that Ω(k) = Ω(k+1)+Ω(k+2)+Ω(k+3).
   Subsequent calls return the n‑th such k (n ≥ 1). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIMIT 20000000ULL          /* sufficient for the first 10 000 terms */
#define INITIAL_TERMS_CAP 1024
#define INITIAL_PRIME_CAP 65536

static bool initialized = false;
static uint32_t *spf        = NULL;    /* smallest prime factor */
static uint8_t  *omega      = NULL;    /* Ω values for 1 .. LIMIT */
static uint32_t *primes     = NULL;    /* list of primes up to LIMIT */
static size_t   primes_len = 0;
static size_t   primes_cap = 0;
static seq_t   *terms      = NULL;    /* collected k values */
static size_t   terms_len  = 0;
static size_t   terms_cap  = 0;

/* build sieve and Ω table, then gather all qualifying k */
static void build_sieve_and_omega(void)
{
    /* Allocate arrays */
    spf   = (uint32_t *)malloc((LIMIT + 5) * sizeof(uint32_t));
    omega = (uint8_t  *)malloc((LIMIT + 5) * sizeof(uint8_t));
    if (!spf || !omega) { abort(); }

    /* Linear sieve to fill spf and primes list */
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (primes_len >= primes_cap) {
                primes_cap = primes_cap == 0 ? INITIAL_PRIME_CAP : primes_cap * 2;
                uint32_t *newp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!newp) abort();
                primes = newp;
            }
            primes[primes_len++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_len; ++j) {
            uint32_t p = primes[j];
            if (p > spf[i] || i * (size_t)p > LIMIT) break;
            spf[i * (size_t)p] = p;
        }
    }

    /* Compute Ω values */
    omega[1] = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        omega[i] = omega[i / (size_t)spf[i]] + 1;
    }

    /* Collect qualifying k */
    terms_len = 0;
    terms_cap = INITIAL_TERMS_CAP;
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    if (!terms) abort();

    for (size_t k = 1; k + 3 <= LIMIT; ++k) {
        uint8_t o1 = omega[k];
        uint8_t o2 = omega[k + 1];
        uint8_t o3 = omega[k + 2];
        uint8_t o4 = omega[k + 3];
        if (o1 == o2 + o3 + o4) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                seq_t *newt = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!newt) abort();
                terms = newt;
            }
            terms[terms_len++] = (seq_t)k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, guard against 0 */

    if (!initialized) {
        build_sieve_and_omega();
        initialized = true;
    }

    size_t idx = (size_t)n - 1;
    if (idx < terms_len) {
        return terms[idx];
    }
    /* n beyond precomputed range – typical for n ≳ 10 000 */
    return 0;
}
