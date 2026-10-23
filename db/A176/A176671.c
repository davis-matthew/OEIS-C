/*
 * A176671  Primes not of the form p + 2^k*k^2 for p prime and k > 0.
 * https://oeis.org/A176671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime q that cannot be written as p + 2^k * k^2
 *            for any prime p and integer k > 0. This function returns that
 *            prime
 * Range:     u8 n <= 17, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ensure_capacity.h"

/* sequential static storage for generated primes and sequence values   */
static uint64_t *prime_list = NULL;
static size_t prime_cap = 0, prime_cnt = 0;

static uint64_t *seq_list = NULL;
static size_t seq_cap = 0, seq_cnt = 0;

static uint64_t last_candidate = 1;   /* candidate to test for primality */

/* ------------------------------------------------------------------ */
/* primality test using the list of primes generated so far            */
/* ------------------------------------------------------------------ */
static bool isPrime(uint64_t x)
{
    if (x < 2) return false;
    // trial division by known primes
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* generate next primes and sequence values until we have at least n   */
/* ------------------------------------------------------------------ */
static void generate_terms(idx_t n)
{
    while (seq_cnt < n) {
        /* find next prime */
        uint64_t cand;
        do {
            ++last_candidate;
            cand = last_candidate;
            if (cand < 2) continue;
        } while (!isPrime(cand));

        /* prime found; add to list of all primes */
        ensure_capacity(prime_cnt + 1, &prime_cap,
                        (void **)&prime_list, sizeof(uint64_t));
        prime_list[prime_cnt++] = cand;

        /* check if cand is representable as p + 2^k*k^2 */
        bool representable = false;
        uint64_t pow2 = 1;   /* 2^k, starts with k=0 */
        for (uint64_t k = 1; ; ++k) {
            /* update 2^k */
            if (pow2 > (UINT64_MAX >> 1)) break; /* avoid overflow on shift */
            pow2 <<= 1;

            /* compute k^2 and test for overflow in product */
            uint64_t k2 = k * k;
            if (pow2 > 0 && pow2 > cand / k2) break;   /* val would exceed cand */

            uint64_t val = pow2 * k2;
            if (val >= cand) break;
            uint64_t p = cand - val;
            if (isPrime(p)) {
                representable = true;
                break;
            }
        }

        if (!representable) {
            ensure_capacity(seq_cnt + 1, &seq_cap,
                            (void **)&seq_list, sizeof(uint64_t));
            seq_list[seq_cnt++] = cand;
        }
    }
}

/* ------------------------------------------------------------------ */
/* public entry point: returns a(n)  (1‑based index)                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for 0, but safety */
    generate_terms(n);
    return (seq_t)seq_list[n - 1];
}
