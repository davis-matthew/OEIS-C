/*
 * A065382  Number of primes between n(n+1)/2 (exclusive) and (n+1)(n+2)/2
 *          (inclusive).
 * https://oeis.org/A065382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3620, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A065382 – Number of primes between n(n+1)/2 (exclusive)
 * and (n+1)(n+2)/2 (inclusive).  Formula:
 *   a(n) = #{p prime : n(n+1)/2 < p ≤ (n+1)(n+2)/2 }  (n ≥ 1)     */

/* The build environment supplies the integer types seq_t and idx_t. */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/* Sieve data – kept static so subsequent calls reuse the allocated
 * memory and do not need to recompute primes each time.
 */
static uint64_t  sieve_limit     = 0;         /* maximum number sieved (inclusive) */
static uint8_t  *sieve_array     = NULL;      /* bit flag: 0 = prime, 1 = composite (odd numbers only) */
static uint32_t *prime_prefix    = NULL;      /* prefix counts of primes among odd entries (2 is counted separately) */
static size_t    sieve_size      = 0;         /* number of odd entries stored */

/* ------------------------------------------------------------------ */
/* Build an Eratosthenes sieve up to 'limit' (inclusive). Only odd
 * numbers are represented: index i corresponds to number 2*i+1.
 */
static void build_sieve(uint64_t limit)
{
    /* Number of odd integers ≤ limit (includes 1 if limit is odd). */
    size_t sz = (limit >> 1) + (limit & 1);

    /* Free any previous allocation. */
    free(sieve_array);
    free(prime_prefix);

    sieve_array  = (uint8_t *)calloc(sz, 1);
    prime_prefix = (uint32_t *)calloc(sz, sizeof(uint32_t));
    sieve_limit  = limit;
    sieve_size   = sz;

    /* 1 is not prime. */
    if (sz > 0)
        sieve_array[0] = 1;

    /* Sieve only for odd numbers. */
    uint64_t r = (uint64_t)sqrt((long double)limit);
    for (uint64_t p = 3; p <= r; p += 2) {
        if (!sieve_array[p >> 1]) {                /* p is prime */
            for (uint64_t mult = p * p; mult <= limit; mult += p << 1)
                sieve_array[mult >> 1] = 1;           /* mark odd composite */
        }
    }

    /* Build prefix counts of odd primes.  2 is handled separately. */
    uint32_t cnt = 0;
    for (size_t idx = 0; idx < sz; ++idx) {
        uint64_t num = (idx << 1) + 1;              /* number represented by idx */
        if (num >= 3 && !sieve_array[idx])
            ++cnt;
        prime_prefix[idx] = cnt;
    }
}

/* ------------------------------------------------------------------ */
/* Return the number of primes ≤ x, assuming x ≤ sieve_limit. */
static seq_t prime_count_upto(uint64_t x)
{
    if (x < 2)
        return 0;

    /* Prime 2 */
    seq_t count = 1;

    if (x >= 3) {
        size_t idx = (x - 1) >> 1;                 /* largest odd ≤ x */
        count += prime_prefix[idx];
    }

    return count;
}

/* ------------------------------------------------------------------ */
/* Sequence definition – external linkage. */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset 1 – sequence undefined below 1 */
        return 0;

    /* Work with 128‑bit arithmetic to avoid overflow on the bounds. */
    __int128 high = (__int128)(n + 1) * (n + 2);
    uint64_t high_incl = (uint64_t)(high / 2);

    __int128 low = (__int128)n * (n + 1);
    uint64_t low_excl = (uint64_t)(low / 2);

    /* Ensure the sieve covers the interval. */
    if (high_incl > sieve_limit)
        build_sieve(high_incl);

    seq_t primes_up_to_high = prime_count_upto(high_incl);
    seq_t primes_up_to_low  = prime_count_upto(low_excl);

    return primes_up_to_high - primes_up_to_low;
}
