/*
 * A204219  Primes whose binary reversal is not prime.
 * https://oeis.org/A204219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204219: a(n) = the nth prime p such that the binary
 * reversal of p is not prime.  The implementation searches forward
 * through primes, testing each reversed value with is_prime_big(). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Reverse the binary representation of a 64‑bit integer.  The loop
 * stops when the original number has no more '1' bits, so leading
 * zeros are dropped. */
static uint64_t rev_bits(uint64_t n)
{
    uint64_t r = 0;
    while (n) {
        r = (r << 1) | (n & 1ULL);
        n >>= 1;
    }
    return r;
}

/* Cached sequence data. */
static seq_t *cache       = NULL;   /* array of qualifying primes */
static size_t cache_len   = 0;      /* number of cached elements   */
static size_t cache_cap   = 0;      /* current capacity            */
static uint64_t last_prime = 0;     /* last prime examined */

/* Grow the cache to hold at least 'needed' elements. */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) abort();   /* OEIS offset is 1 – undefined for n=0 */

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len) {
        return cache[(size_t)n - 1];
    }

    /* Extend cache until it contains n elements. */
    while (cache_len < (size_t)n) {
        /* Find the next prime. */
        uint64_t p;
        if (last_prime == 0) {
            p = 2;
        } else {
            p = next_prime(last_prime + 1);
        }
        last_prime = p;

        /* Test the binary reversal for primality. */
        uint64_t rev = rev_bits(p);
        bool rev_is_prime = is_prime_big((seq_t)rev);

        if (!rev_is_prime) {
            /* Store the qualifying prime. */
            ensure_cache_size(cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[(size_t)n - 1];
}
