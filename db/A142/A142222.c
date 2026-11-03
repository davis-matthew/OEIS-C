/*
 * A142222  Primes congruent to 25 mod 41.
 * https://oeis.org/A142222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p ≡ 25 (mod 41)
 * Range:     u8 n <= 1, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 914 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate candidates 41·k + 25 for k ≥ 2 and test for primality,
   caching the found primes so that any order of calls runs in O(n) time. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

static seq_t *prime_cache = NULL;   /* array of cached primes */
static size_t prime_cap   = 0;      /* allocated capacity */
static size_t prime_len   = 0;      /* number of cached primes */
static idx_t  next_k      = 2;      /* k for next candidate 41·k + 25 */

/* Append a prime to the cache, resizing the array as needed. */
static void append_prime(seq_t val) {
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        seq_t *new_arr = alloc_seq_array(new_cap);
        /* copy existing elements */
        size_t i;
        for (i = 0; i < prime_len; ++i)
            new_arr[i] = prime_cache[i];
        if (prime_cache)
            free_seq_array(prime_cache);
        prime_cache = new_arr;
        prime_cap   = new_cap;
    }
    prime_cache[prime_len++] = val;
}

/* Ensure that at least n primes have been cached. */
static void ensure_cache(idx_t n) {
    while (prime_len < n) {
        seq_t cand = (seq_t)41 * (seq_t)next_k + (seq_t)25;
        if (is_prime_big(cand))
            append_prime(cand);
        ++next_k;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* guard against n < 1, though offset is 1 */
    ensure_cache(n);
    return prime_cache[n - 1];
}
