/*
 * A117049  Primes of the form 22*(n^2)+1.
 * https://oeis.org/A117049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function yields the nth prime of the form 22*k^2+1
   by searching successive integers k.  The sequence is not
   given by a closed formula; instead we generate primes on
   demand, caching the results for subsequent calls. */

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

static seq_t *cache = NULL;   /* array of found primes */
static idx_t  cached_count = 0;   /* how many primes we have cached */
static idx_t  k_next = 1;       /* next k to test */
static size_t cache_capacity = 0;

static void ensure_cache_capacity(idx_t min_needed)
{
    if (cache_capacity >= (size_t)min_needed)
        return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
    while (new_cap < (size_t)min_needed)
        new_cap *= 2;

    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    if (new_cache == NULL) {
        /* In a production environment you might handle the error differently.
           For this exercise we simply abort. */
        abort();
    }
    cache = new_cache;
    cache_capacity = new_cap;
}

static void extend_sequence_to(idx_t target)
{
    while (cached_count < target) {
        seq_t val;
        /* find the next k such that 22*k^2+1 is prime */
        for (;;) {
            val = (seq_t)22 * (seq_t)k_next * (seq_t)k_next + 1;
            if (is_prime_big(val))
                break;
            ++k_next;
        }
        /* store the found prime */
        ensure_cache_capacity(cached_count + 1);
        cache[cached_count++] = val;
        ++k_next;   /* move to the next candidate for future searches */
    }
}

seq_t a(idx_t n)
{
    if (n < 1)   /* the sequence is only defined for n >= 1 */
        return 0;

    if (cached_count < n)
        extend_sequence_to(n);

    return cache[n - 1];
}
