/*
 * A040102  Primes p such that x^4 = 3 has no solution mod p.
 * https://oeis.org/A040102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth prime p such that the congruence
   x^4 ≡ 3 (mod p) has no solution.  Equivalently, 3 is a fourth
   power non‑residue modulo p. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"

/* Cache of primes and qualifying primes.  They are recomputed
   only when the requested index exceeds the current cache size. */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static uint64_t sieve_limit = 200000;   /* initial sieve size */

static seq_t *qual_primes = NULL;
static size_t qual_count = 0;

/* Rebuild the cache up to a given limit.  All previous data are
   discarded and replaced by fresh results. */
static void rebuild_cache(uint64_t limit)
{
    /* Generate all primes up to limit. */
    uint64_t *temp = NULL;
    size_t temp_count = 0;
    build_primes_upto(limit, &temp, &temp_count);

    /* Count qualifying primes. */
    size_t q = 0;
    for (size_t i = 0; i < temp_count; ++i) {
        uint64_t p = temp[i];
        if (p <= 3) continue;               /* 2 and 3 are excluded */

        /* Determine gcd(4, p-1).  If p ≡ 1 mod 4, gcd = 4; otherwise 2. */
        uint64_t g = (p % 4 == 1) ? 4 : 2;
        uint64_t exp = (p - 1) / g;

        /* 3 is a fourth power residue iff 3^exp ≡ 1 (mod p). */
        seq_t r = mod_pow((seq_t)3, exp, (seq_t)p);
        if (r != (seq_t)1) {
            ++q;
        }
    }

    /* Allocate new array for qualifying primes. */
    seq_t *new_q = (seq_t *)malloc(q * sizeof(seq_t));
    size_t idx = 0;
    for (size_t i = 0; i < temp_count; ++i) {
        uint64_t p = temp[i];
        if (p <= 3) continue;

        uint64_t g = (p % 4 == 1) ? 4 : 2;
        uint64_t exp = (p - 1) / g;
        seq_t r = mod_pow((seq_t)3, exp, (seq_t)p);
        if (r != (seq_t)1) {
            new_q[idx++] = (seq_t)p;
        }
    }

    /* Free old data and replace with new. */
    free(prime_list);
    free(qual_primes);
    prime_list = temp;
    prime_count = temp_count;
    qual_primes = new_q;
    qual_count = q;
    sieve_limit = limit;
}

/* Ensure that qual_primes contains at least n elements.
   If not, increase the sieve limit and rebuild. */
static void ensure_cache(idx_t n)
{
    if (qual_count >= (size_t)n) return;

    uint64_t limit = sieve_limit;
    while (qual_count < (size_t)n) {
        limit = limit * 2;          /* increase size */
        rebuild_cache(limit);
    }
}

/* External sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* behaviour below offset not needed */

    ensure_cache(n);
    return qual_primes[(size_t)(n - 1)];
}
