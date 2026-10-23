/*
 * A142409  Primes congruent to 35 mod 48.
 * https://oeis.org/A142409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that p ≡ 35 (mod 48)
 * Range:     u8 n <= 4, u16 n <= 409, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code builds up the needed primes on first use, caching them so
   repeated calls are fast.  All arithmetic is performed with 64‑bit
   unsigned integers for the internal sieve; the result is cast to
   seq_t before returning. */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *primes_u64 = NULL;     /* all primes found so far          */
static size_t   primes_len  = 0;
static size_t   primes_cap  = 0;

static uint64_t *good_u64    = NULL;     /* primes ≡ 35 (mod 48)              */
static size_t   good_len    = 0;
static size_t   good_cap    = 0;

static uint64_t last_candidate = 1;      /* last number examined */

static void grow_primes(void)
{
    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
    uint64_t *new_ptr =
        (uint64_t *)realloc(primes_u64, new_cap * sizeof(uint64_t));
    if (!new_ptr) abort();
    primes_u64 = new_ptr;
    primes_cap = new_cap;
}

static void grow_good(void)
{
    size_t new_cap = good_cap ? good_cap * 2 : 16;
    uint64_t *new_ptr =
        (uint64_t *)realloc(good_u64, new_cap * sizeof(uint64_t));
    if (!new_ptr) abort();
    good_u64 = new_ptr;
    good_cap = new_cap;
}

static void add_prime(uint64_t p)
{
    if (primes_len == primes_cap) grow_primes();
    primes_u64[primes_len++] = p;
}

static void add_good_prime(uint64_t p)
{
    if (good_len == good_cap) grow_good();
    good_u64[good_len++] = p;
}

static int is_prime_uint64(uint64_t x)
{
    if (x < 2) return 0;
    if (x == 2 || x == 3) return 1;
    if ((x & 1) == 0) return 0;  /* even check */

    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t q = primes_u64[i];
        if ((unsigned __int128)q * q > (unsigned __int128)x) break;
        if (x % q == 0) return 0;
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n == 0 */

    /* If we already have the n‑th good prime, return it. */
    if ((size_t)n <= good_len)
        return (seq_t)good_u64[(size_t)n - 1];

    /* Generate primes until we have at least n good primes. */
    uint64_t candidate = last_candidate + 1;
    while (good_len < (size_t)n) {
        if (is_prime_uint64(candidate)) {
            add_prime(candidate);
            if (candidate % 48 == 35)
                add_good_prime(candidate);
        }
        last_candidate = candidate;
        ++candidate;
    }

    return (seq_t)good_u64[(size_t)n - 1];
}
