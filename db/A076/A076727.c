/*
 * A076727  Primes of the form x^2 + (x+3)^2.
 * https://oeis.org/A076727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 840 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A076727:
 *   primes of the form x² + (x+3)²
 *   Each such prime p can be written uniquely as x² + y² with y−x = 3.
 *   The function returns the nth such prime (offset 1).  If n == 0 it
 *   returns 0; otherwise it uses memoisation so calls in any order
 *   remain fast.
 */
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

/* Cache of primes found so far.  cache[i] holds the (i+1)-st prime. */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
/* Next x to test.  x starts at 1 because x=0 gives 9, not prime. */
static idx_t next_x = 1;
static bool inited   = false;

/* Ensure the cache can hold at least needed items. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    cache = mem_resize(seq_t, cache, new_cap);
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* no term for offset 0 */

    if (!inited) {
        cache = NULL;
        cache_cap = 0;
        cache_len = 0;
        next_x = 1;
        inited = true;
    }

    /* generate primes until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t x   = (seq_t)next_x;
        seq_t val = (seq_t)2 * x * x + (seq_t)6 * x + 9;  /* x² + (x+3)² */

        if (is_prime_big(val)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = val;
        }
        next_x++;
    }

    return cache[n - 1];
}
