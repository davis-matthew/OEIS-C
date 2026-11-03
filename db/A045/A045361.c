/*
 * A045361  Primes congruent to {2, 4, 5, 6} mod 7.
 * https://oeis.org/A045361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 958 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045361: primes congruent to 2,4,5 or 6 mod 7
   a(n) = the nth prime p such that p % 7 ∈ {2,4,5,6}  (n ≥ 1)  */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

/* static storage for cached primes */
static seq_t *prime_cache = NULL;   /* array of found primes  */
static size_t cache_size = 0;       /* number of primes stored */
static size_t cache_cap  = 0;       /* capacity of prime_cache  */
static uint64_t next_candidate = 2; /* next number to test */

/* test whether a 64‑bit number has a residue allowed by the sequence */
static inline bool good_residue(uint64_t v) {
    uint64_t r = v % 7;
    return r == 2 || r == 4 || r == 5 || r == 6;
}

/* enlarge the cache to at least 'needed' elements */
static void grow_cache(size_t needed) {
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)seq_realloc(prime_cache, cache_cap, new_cap);
    prime_cache = new_cache;
    cache_cap  = new_cap;
}

/* sequence generator */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;               /* undefined for n < 1, but any value works */

    while (cache_size < (size_t)n) {
        /* skip numbers that cannot be in the sequence */
        while (!good_residue(next_candidate))
            ++next_candidate;

        /* test the current candidate for primality */
        if (is_prime_big((seq_t)next_candidate)) {
            grow_cache(cache_size + 1);
            prime_cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return prime_cache[n - 1];
}
