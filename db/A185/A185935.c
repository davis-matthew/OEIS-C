/*
 * A185935  Lesser of two consecutive primes which both equal 2 (mod 3).
 * https://oeis.org/A185935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the lesser of consecutive primes p and q
   such that p ≡ 2 (mod 3), q ≡ 2 (mod 3). */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

/* Helper: return the next prime strictly greater than cur. */
static seq_t next_prime_generic(seq_t cur)
{
    seq_t nxt = cur + 1;
    while (!is_prime_big(nxt))
        nxt += 1;
    return nxt;
}

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* Static state of the prime generator */
    static seq_t prev_prime = 0, curr_prime = 0;
    static bool initialized = false;

    /* Initialisation */
    if (!initialized) {
        prev_prime = 2;
        curr_prime = 3;          /* first prime after 2   */
        initialized = true;

        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();
    }

    /* Extend cache until we have n terms */
    while (cache_len < n) {
        /* If the consecutive pair satisfies the condition,
           store the lesser prime. */
        if ((prev_prime % 3U == 2U) && (curr_prime % 3U == 2U)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = prev_prime;
        }

        /* Advance to the next pair of consecutive primes. */
        prev_prime = curr_prime;
        curr_prime = next_prime_generic(curr_prime);
    }

    return cache[n - 1];
}
