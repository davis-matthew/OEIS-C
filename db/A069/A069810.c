/*
 * A069810  Integers k such that gcd(k, sigma(k)) = tau(k).
 * https://oeis.org/A069810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 871, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, gcd, mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A069810:
   integers k such that gcd(k, sigma(k)) = tau(k). The sequence is
   indexed starting at 1 (a(1) = 1). This implementation uses a
   dynamic sieve of primes and factorises each integer on the fly,
   computing sigma modulo n to avoid huge intermediate values. */
/* Note: a sequence value is returned as a seq_t. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/mul_mod64.h"

/* Static storage for generated terms of the sequence */
static seq_t *seq_terms = NULL;
static size_t seq_cnt   = 0;
static size_t seq_cap   = 0;

/* Static storage for the prime table used in factorisation */
static uint64_t *primes     = NULL;
static size_t    primes_cnt = 0;

/* Last integer examined by the sieve */
static uint64_t last_checked = 0;

/* Ensure that we have all primes up to 'limit' */
static void ensure_primes_up_to(uint64_t limit)
{
    if (primes == NULL || primes_cnt == 0 || primes[primes_cnt - 1] < limit) {
        if (primes != NULL) free(primes);
        primes = NULL;
        primes_cnt = 0;
        build_primes_upto(limit, &primes, &primes_cnt);
    }
}

/* Test whether n satisfies the sequence condition.
   Computes sigma(k) modulo k to keep intermediate numbers small. */
static bool passes_condition(uint64_t n)
{
    uint64_t temp = n;
    seq_t tau = 1;
    uint64_t sigma_mod = 1 % n;  /* starts as 1 mod n */

    uint64_t limit = _floor_isqrt_u64(n);
    ensure_primes_up_to(limit);

    for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            /* Compute sum_{j=0..exp} p^j  (mod n) */
            uint64_t sum_mod = 1 % n;
            uint64_t mult   = 1 % n;
            for (int j = 0; j < exp; ++j) {
                mult = mul_mod64(mult, p, n);
                sum_mod += mult;
                if (sum_mod >= n) sum_mod -= n;
            }
            /* Update sigma_mod = sigma_mod * sum_mod (mod n) */
            sigma_mod = mul_mod64(sigma_mod, sum_mod, n);

            /* Update tau */
            tau *= (seq_t)(exp + 1);
        }
    }

    if (temp > 1) { /* remaining prime factor */
        uint64_t sum_mod = (1 + (temp % n));
        if (sum_mod >= n) sum_mod -= n;
        sigma_mod = mul_mod64(sigma_mod, sum_mod, n);
        tau *= 2;
    }

    seq_t g = gcd((seq_t)n, (seq_t)sigma_mod);
    return g == tau;
}

/* Ensure that we have enough storage for seq_terms. */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        while (new_cap < needed) new_cap *= 2;
        seq_terms = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
        seq_cap   = new_cap;
    }
}

/* Return the n-th term of the sequence (with a(1) = first term). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* out of bounds, not used */

    /* Generate terms until we have the required amount */
    while (seq_cnt < (size_t)n) {
        ++last_checked;
        if (passes_condition(last_checked)) {
            ensure_seq_capacity(seq_cnt + 1);
            seq_terms[seq_cnt++] = (seq_t)last_checked;
        }
    }

    return seq_terms[(size_t)n - 1];
}
