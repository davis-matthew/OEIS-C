/*
 * A201017  Primes whose multiplicative digital root is 5.
 * https://oeis.org/A201017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   digit_product, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose multiplicative digital root is 5
   a(n) = the n-th prime p with mdroot(p) = 5, n >= 1   (a(1) = 5) */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include <stdint.h>
#include "../utils/digit_product.h"
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

/* static cache of qualifying primes */
static seq_t *prime_cache = NULL;
static size_t cache_len   = 0;   /* number of primes stored   */
static size_t cache_cap   = 0;   /* allocated capacity        */
static seq_t next_candidate = 2; /* next integer to test      */

/* Expand the cache when necessary */
static void grow_cache(void) {
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0 ? 64 : cache_cap * 2);
        seq_t *new_buf = seq_alloc(new_cap);
        for (size_t i = 0; i < cache_len; ++i) {
            new_buf[i] = prime_cache[i];
        }
        prime_cache = new_buf;
        cache_cap   = new_cap;
    }
}

/* Multiplicative digital root */
static seq_t mdroot(seq_t v) {
    while (v >= 10) {
        v = digit_product(v);
    }
    return v;
}

/* Main function required by the build system */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined, safe fallback */

    /* Search until we have the first n qualifying primes */
    while (cache_len < (size_t)n) {
        /* Test the current candidate */
        if (is_prime_big(next_candidate) &&
            mdroot(next_candidate) == 5) {
            grow_cache();
            prime_cache[cache_len++] = next_candidate;
        }
        /* Move to the next integer */
        next_candidate = next_candidate + 1;
    }
    return prime_cache[n - 1];
}
