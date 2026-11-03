/*
 * A142182  Primes congruent to 38 mod 39.
 * https://oeis.org/A142182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 275, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 962 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 38 (mod 39)
   a(n) returns the nth prime p such that p % 39 == 38,
   with a(1) = 233. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

/* Cache of found primes */
static seq_t *prime_cache   = NULL;
static size_t prime_len     = 0;
static size_t prime_cap     = 0;

/* Next candidate of the form 39*k + 38, starting from 38 */
static uint64_t next_candidate = 38;

/* Smallest positive integer > 0 that fits in seq_t (used as an upper bound). */
static const uint64_t max_seq_t_value = (uint64_t)((seq_t)-1);

/* Ensure the cache can hold at least one more element. */
static void ensure_cache_capacity(void)
{
    if (prime_len >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
        if (!tmp) {
            /* Allocation failed – clean up and leave the cache empty. */
            mem_free(prime_cache);
            prime_cache = NULL;
            prime_cap   = 0;
            prime_len   = 0;
            return;
        }
        prime_cache = tmp;
        prime_cap   = new_cap;
    }
}

/* Return the nth prime congruent to 38 (mod 39), with n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, value irrelevant */

    while (prime_len < n) {
        /* Stop early if the candidate cannot fit into seq_t. */
        if (next_candidate > max_seq_t_value) {
            /* No more representable primes – return 0 or abort. */
            return (seq_t)0;
        }

        ensure_cache_capacity();
        if (!prime_cache) return (seq_t)0;   /* allocation failed */

        /* Test the candidate for primality and store it if it is prime. */
        if (is_prime_big((seq_t)next_candidate)) {
            prime_cache[prime_len++] = (seq_t)next_candidate;
        }
        next_candidate += 39;   /* Next number of the form 39*k + 38 */
    }

    return prime_cache[n - 1];
}
