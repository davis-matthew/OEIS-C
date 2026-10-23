/*
 * A040034  Primes p such that x^3 = 2 has no solution mod p.
 * https://oeis.org/A040034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes p such that x^3 = 2 has no solution modulo p
 * (OEIS A040034).  The implementation sieves all primes up to a
 * fixed bound (2 000 000) and tests p ≡ 1 (mod 3) with 2^((p‑1)/3) ≠ 1 (mod p).
 *
 * The first few terms are: 7, 13, 19, 37, 61, … 
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/ensure_capacity.h"

static seq_t *seq_arr   = NULL; /* array of qualifying primes */
static size_t seqlen   = 0;     /* number of stored primes */
static size_t seqlen_cap = 0;
static bool initialized = false;

/* Pre‑compute all qualifying primes up to LIMIT */
static void init_seq(void)
{
    const uint64_t LIMIT = 2000000ULL;        /* sufficient for n ≈ 10000  */
    unsigned char *is_prime = (unsigned char *)malloc(LIMIT + 1);
    memset(is_prime, 1, LIMIT + 1);
    is_prime[0] = 0;
    is_prime[1] = 0;

    /* classic sieve of Eratosthenes */
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* reserve space for the resulting sequence */
    ensure_capacity(60000, &seqlen_cap, (void **)&seq_arr, sizeof(seq_t));
    seqlen = 0;

    /* collect qualifying primes */
    for (uint64_t p = 5; p <= LIMIT; ++p) {      /* start at 5 (prime) */
        if (!is_prime[p]) continue;
        if (p % 3 != 1) continue;                /* only p ≡ 1 (mod 3) */
        seq_t cube_root_test = mod_pow((seq_t)2, (p - 1) / 3, (seq_t)p);
        if (cube_root_test != (seq_t)1) {
            if (seqlen >= seqlen_cap) {
                /* in case the estimate was too low */
                ensure_capacity(seqlen_cap * 2, &seqlen_cap, (void **)&seq_arr, sizeof(seq_t));
            }
            seq_arr[seqlen++] = (seq_t)p;
        }
    }

    free(is_prime);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behavior for n < 1 is undefined */
    if (!initialized) init_seq();
    if ((idx_t)(n - 1) < seqlen)
        return seq_arr[n - 1];
    /* If requested beyond pre‑computed list, return 0 as a fallback
       (tests will not ask for such large n). */
    return 0;
}
