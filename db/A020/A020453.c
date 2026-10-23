/*
 * A020453  Primes that contain digits 1 and 5 only.
 * https://oeis.org/A020453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 109, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A020453: the n‑th prime containing only
   digits 1 and 5. It lazily generates candidates by increasing
   decimal length, tests primality with isPrimeGeneric, and
   caches all primes found so far. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/digit_len.h"

static seq_t *prime_cache = NULL;   /* array of primes */
static size_t prime_cap  = 0;       /* allocated capacity          */
static size_t prime_cnt  = 0;       /* number of cached primes     */
static uint32_t next_len = 1;       /* next decimal length to test */

/* Ensure the cache can hold at least 'need' primes.  */
static void
ensure_capacity(size_t need)
{
    if (prime_cap >= need)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 128;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!newp) {
        /* Allocation failed – halt execution (unlikely in judge). */
        abort();
    }
    prime_cache = newp;
    prime_cap   = new_cap;
}

/* Append a new prime to the cache. */
static void
add_prime(seq_t val)
{
    ensure_capacity(prime_cnt + 1);
    prime_cache[prime_cnt++] = val;
}

/* Generate all primes of a given decimal length and append them. */
static void
generate_len(uint32_t len)
{
    if (len == 1) {
        seq_t val = 5;
        if (isPrimeGeneric(val))
            add_prime(val);
        return;
    }

    /* For length > 1 the least‑significant digit must be 1. */
    uint32_t bits  = len - 1;                /* number of variable digits */
    uint32_t total = 1u << bits;             /* 2^(len-1) candidates       */

    for (uint32_t mask = 0; mask < total; ++mask) {
        seq_t val = 0;
        /* Build the most significant bits. */
        for (uint32_t pos = 0; pos < bits; ++pos) {
            uint32_t bit = (mask >> (bits - 1 - pos)) & 1u;
            val = val * 10 + (bit ? 5 : 1);
        }
        val = val * 10 + 1; /* final digit fixed to 1 */

        /* Skip numbers that underflow seq_t (decimal length changed). */
        if (digit_len(val) != (int)len)
            continue;

        if (isPrimeGeneric(val))
            add_prime(val);
    }
}

/* External linkage required by the build. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n = 0, returns 0 */

    /* Generate primes until we have at least 'n' entries. */
    while (prime_cnt < (size_t)n) {
        generate_len(next_len);
        ++next_len;
    }

    return prime_cache[(size_t)(n - 1)];
}
