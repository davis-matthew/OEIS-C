/*
 * A174916  Lesser of twin primes p1 such that p1 + p2^2 - p1^2 is a prime
 *          number.
 * https://oeis.org/A174916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 252, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 974 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174916:
   Lesser of twin primes p for which 5*p+4 is also prime.
   The function scans successive integers, tests primality with
   the helper is_prime_big, and caches the result. */

#include <stddef.h>
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

/* Cache for the computed terms */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_len = 0;         /* number of cached terms */
static size_t cache_cap = 0;         /* allocated capacity */

/* Next candidate to test (odd integers only). Initialized to 3. */
static idx_t next_candidate = 3;

/* Ensure that cache contains at least n terms (1‑based). */
static void ensure_n_terms(idx_t n)
{
    /* Reserve space for new terms first. */
    while (cache_len < (size_t)n) {
        /* Expand cache if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Test the odd candidate. */
        if (is_prime_big((seq_t)next_candidate) &&
            is_prime_big((seq_t)(next_candidate + 2)) &&
            is_prime_big((seq_t)(5 * next_candidate + 4))) {
            cache[cache_len++] = (seq_t)next_candidate;
        }

        /* Move to the next odd number. */
        next_candidate += 2;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)          /* Undefined for n = 0; return 0 as a safeguard. */
        return (seq_t)0;

    ensure_n_terms(n);   /* Fill cache up to n. */
    return cache[(size_t)n - 1];
}
