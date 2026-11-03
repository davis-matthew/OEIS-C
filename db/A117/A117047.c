/*
 * A117047  Primes of the form 60*k + 11.
 * https://oeis.org/A117047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 415, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 991 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime of the form 60*k + 11
   (a(1) = 11).  Calls are independent of order; subsequent calls
   are answered from a growing cache.  The implementation relies
   on the provided generic primality routine is_prime_big. */
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

static seq_t *cache = NULL;          /* cached primes of the form 60k+11   */
static size_t cache_len = 0;         /* number of elements in cache       */
static size_t cache_cap = 0;         /* allocated capacity of cache       */
static idx_t current_k = 0;          /* next k to test (candidate = 60*k+11) */

static void grow_cache(void)
{
    if (cache_cap < cache_len + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) {
            /* Allocation failed – abort to avoid undefined behaviour */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* offset is 1, so n must be >= 1 */
    if (n == 0) return 0;

    /* If we already have the value cached, return it */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Generate further primes until we reach the nth one */
    while (cache_len < (size_t)n) {
        seq_t candidate = (seq_t)60 * current_k + (seq_t)11;
        if (is_prime_big(candidate)) {
            grow_cache();
            cache[cache_len++] = candidate;
        }
        ++current_k;
    }

    return cache[n - 1];
}
