/*
 * A075280  Interprimes which are of the form s*prime, s=5.
 * https://oeis.org/A075280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th Interprime of the form 5*prime.
   Search for pairs of consecutive primes (p,q), take
   m = (p+q)/2 and keep those m that are divisible by 5
   with m/5 also prime.  The first such m is 15. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include <math.h>
#include "../utils/next_prime.h"
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static uint64_t prev_prime = 0;
static uint64_t next_prime_val = 0;
static bool initialized = false;

/* Grow the cache to hold at least min_cap elements. */
static void grow_cache(size_t min_cap)
{
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *new_arr = seq_alloc_new_array(new_cap);
        if (cache && cache_len) {
            mem_copy(new_arr, cache, cache_len * sizeof(seq_t));
        }
        if (cache) seq_free_array(cache);
        cache = new_arr;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;
        prev_prime = 3ULL;
        next_prime_val = 5ULL;
    }

    if (cache_len >= (size_t)n) {
        return cache[(size_t)n - 1];
    }

    while (cache_len < (size_t)n) {
        seq_t m = ((seq_t)prev_prime + (seq_t)next_prime_val) / 2;

        if (m % (seq_t)5 == 0) {
            seq_t q = m / (seq_t)5;
            if (is_prime_big(q)) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = m;
            }
        }

        prev_prime = next_prime_val;
        next_prime_val = next_prime(prev_prime);
    }

    return cache[(size_t)n - 1];
}
