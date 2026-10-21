/*
 * A095318  Primes in whose binary expansion the number of 1 bits is > 3 +
 *          number of 0 bits.
 * https://oeis.org/A095318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A095318 – Primes in whose binary expansion the number of 1 bits
   is greater than 3 plus the number of 0 bits.  The function generates
   the terms by testing each prime in increasing order and keeping those
   that satisfy the above property.  No look‑up table is used. */

#include <stdint.h>
#include <stdlib.h>

/* --- global static state ------------------------------------------------- */
static uint64_t *prime_cache = NULL;   /* primes found so far (uint64_t)   */
static size_t   prime_cnt   = 0;
static size_t   prime_cap   = 0;

static seq_t   *good_cache  = NULL;    /* primes that satisfy the property */
static size_t   good_cnt    = 0;
static size_t   good_cap    = 0;

/* --- helper: ensure enough capacity ------------------------------------- */
static void ensure_prime_cap(size_t needed)
{
    if (needed <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap : 64;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t*)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_cache = tmp;
    prime_cap   = new_cap;
}

static void ensure_good_cap(size_t needed)
{
    if (needed <= good_cap) return;
    size_t new_cap = good_cap ? good_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(good_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    good_cache = tmp;
    good_cap   = new_cap;
}

/* --- simple primality test (trial division using collected primes) -------- */
static int is_prime(uint64_t n)
{
    if (n < 2) return 0;
    if (n == 2) return 1;
    if ((n & 1) == 0) return 0;
    uint64_t r = (uint64_t)((double)n * 0.5);      /* sqrt approximation */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p > r) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

/* --- bit operations ------------------------------------------------------ */
static unsigned int popcount64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return (unsigned int)__builtin_popcountll(x);
#else
    unsigned int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
#endif
}

static unsigned int bitlen64(uint64_t x)
{
    if (x == 0) return 0;
#if defined(__GNUC__) || defined(__clang__)
    return 64 - __builtin_clzll(x);
#else
    unsigned int bl = 0;
    while (x) { x >>= 1; ++bl; }
    return bl;
#endif
}

/* Check whether a prime satisfies the property */
static int good_prime(uint64_t p)
{
    unsigned int ones = popcount64(p);
    unsigned int bits = bitlen64(p);
    unsigned int zeros = bits - ones;
    return ones > 3 + zeros;
}

/* --- main sequence generator --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety: sequence starts at 1 */

    /* Return a previously cached value if we have it */
    if (good_cnt >= (size_t)n) return good_cache[n-1];

    /* Ensure we have enough space for new entries */
    ensure_prime_cap(prime_cnt + 128);
    ensure_good_cap(good_cnt + 128);

    /* Start searching for primes from the smallest needed candidate */
    uint64_t candidate = (prime_cnt == 0) ? 2 : prime_cache[prime_cnt-1] + 1;
    if (candidate == 2) {
        /* Add 2 first, then move to the next odd number */
        if (is_prime(candidate)) {
            prime_cache[prime_cnt++] = candidate;
            if (good_prime(candidate)) {
                good_cache[good_cnt++] = (seq_t)candidate;
                if (good_cnt >= (size_t)n) return good_cache[n-1];
            }
        }
        candidate = 3;           /* next prime candidate */
    } else if ((candidate & 1) == 0) {
        /* Skip even numbers after 2 */
        candidate++;
    }

    /* Keep finding primes until we have n good ones */
    while (good_cnt < (size_t)n) {
        if (is_prime(candidate)) {
            prime_cache[prime_cnt++] = candidate;
            if (good_prime(candidate)) {
                good_cache[good_cnt++] = (seq_t)candidate;
                if (good_cnt >= (size_t)n) return good_cache[n-1];
            }
        }
        candidate += 2;          /* only odd numbers need testing */
    }

    /* The loop above guarantees we return before reaching this point. */
    return good_cache[n-1];
}
