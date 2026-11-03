/*
 * A087915  Even numbers k such that 2*k+3 is a prime.
 * https://oeis.org/A087915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Even numbers k such that 2*k+3 is a prime. */
/* a(n) returns the nth such even k, offset 1. */

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

static seq_t *cache      = NULL;   /* array of computed terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* current capacity */
static idx_t  next_k     = 0;      /* next even k to test */

/* Grow the cache when it is full. */
static void grow_cache_if_needed(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        cache = mem_resize(seq_t, cache, new_cap);
        cache_cap = new_cap;
    }
}

/* Fill the cache until it contains at least `target` terms. */
static void ensure_cache(idx_t target)
{
    if (cache_cap == 0) {           /* first allocation */
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
    }
    while (cache_len < target) {
        seq_t prime_candidate = (seq_t)2 * next_k + 3;
        if (is_prime_big(prime_candidate)) {
            grow_cache_if_needed();
            cache[cache_len++] = (seq_t)next_k;
        }
        next_k += 2;   /* only even k are considered */
    }
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n<1, safe fallback */
    ensure_cache(n);
    return cache[n - 1];
}
