/*
 * A141908  Primes congruent to 2 mod 23.
 * https://oeis.org/A141908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 296, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 987 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 2 mod 23 (search) */
#include <stdbool.h>
#include <stdint.h>
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

/* Cached values of the sequence. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of stored terms */
static size_t cache_cap = 0;   /* allocated capacity */
static uint64_t next_candidate = 2;   /* next integer to test */

/* Ensure that the cache has at least 'need' slots. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_arr = mem_resize(seq_t, cache, new_cap);
    cache = new_arr;
    cache_cap = new_cap;
}

/* Extend the cache until it contains at least n terms. */
static void generate_terms_up_to(idx_t n)
{
    /* guard against unsigned wrap for n == 0 (undefined behaviour) */
    if (n == 0)
        return;

    while (cache_len < n) {
        /* Check modulo 23 and primality. */
        if ((next_candidate % 23ULL) == 2ULL && is_prime_big((seq_t)next_candidate)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        /* Advance to the next candidate. */
        ++next_candidate;
    }
}

/* PUBLIC API --------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* The sequence is 1-indexed; behaviour for n < 1 is undefined. */
    generate_terms_up_to(n);
    return cache[n - 1];
}
