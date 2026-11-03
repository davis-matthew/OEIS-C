/*
 * A142286  Primes congruent to 37 mod 43.
 * https://oeis.org/A142286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 930 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A142286: primes congruent to 37 (mod 43). */
/* a(n) is the n‑th prime p with p ≡ 37 (mod 43), n ≥ 1. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"
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

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

static seq_t *prime_cache = NULL;   /* array of found primes */
static idx_t   prime_cnt   = 0;     /* number of stored primes   */
static idx_t   candidate   = 37;    /* first number to test      */
static size_t cache_cap   = 0;      /* capacity of prime_cache  */

/* Grow the cache so it can hold at least 'need' elements. */
static void grow_cache(idx_t need)
{
    if (cache_cap >= (size_t)need)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < (size_t)need)
        new_cap <<= 1;

    seq_t *newp = alloc_seq_array(new_cap);
    for (idx_t i = 0; i < prime_cnt; ++i)
        newp[i] = prime_cache[i];

    if (prime_cache)
        seq_free_array(prime_cache);

    prime_cache = newp;
    cache_cap   = new_cap;
}

/* Return the n-th prime that is 37 (mod 43).  Undefined for n = 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* out of range, keep behaviour benign */

    /* If we already have enough primes, just return the cached value. */
    if (n <= prime_cnt)
        return prime_cache[n - 1];

    /* Search for additional primes until we have at least n of them. */
    while (prime_cnt < n) {
        if (is_prime_big((seq_t)candidate)) {
            grow_cache(prime_cnt + 1);
            prime_cache[prime_cnt++] = (seq_t)candidate;
        }
        candidate += 43;
    }

    return prime_cache[n - 1];
}
