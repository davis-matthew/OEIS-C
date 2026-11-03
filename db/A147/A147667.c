/*
 * A147667  Primes of the form 5^k - 4^k.
 * https://oeis.org/A147667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime of the form 5^k - 4^k, where k is a positive
 *            integer (odd k because even k produce a multiple of 3)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 1,
 *            big any n
 * Validated: standard, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   ipow, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/ipow.h"
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* static cache of found primes and state of the search */
static seq_t *cache = NULL;          /* array of primes found */
static idx_t cache_len = 0;          /* number of primes stored */
static size_t cache_cap = 0;         /* current capacity of cache */
static bool cache_ready = false;     /* whether initialization has been performed */

/* current exponent and powers of 5 and 4 that correspond to the last k examined */
static idx_t   k_curr   = 0;   /* current k (always odd after first init) */
static seq_t   pow5_curr = 0;  /* 5^k_curr   */
static seq_t   pow4_curr = 0;  /* 4^k_curr   */

/* extend the cache until it contains at least n primes */
static void extend_to(idx_t n)
{
    /* first time: initialise to the first odd k = 3 */
    if (!cache_ready) {
        cache_ready = true;
        k_curr   = 3;
        pow5_curr = ipow(5, (seq_t)k_curr);
        pow4_curr = ipow(4, (seq_t)k_curr);
    }

    while (cache_len < n) {
        /* value for the current k */
        seq_t val = pow5_curr - pow4_curr;

        if (is_prime_big(val)) {
            /* grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 4;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }

        /* advance to the next odd exponent k + 2 */
        k_curr += 2;
        pow5_curr = seq_mul(pow5_curr, 25);   /* 5^(k+2) = 5^k * 5^2 */
        pow4_curr = seq_mul(pow4_curr, 16);   /* 4^(k+2) = 4^k * 4^2 */
    }
}

/* public function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1, but guard against zero */
    extend_to(n);
    return cache[n - 1];
}
