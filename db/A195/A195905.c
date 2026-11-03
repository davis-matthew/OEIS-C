/*
 * A195905  Primes of the form 10 * k^2 + 7.
 * https://oeis.org/A195905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 37, u32 n <= 3932, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 49 terms through n = 4837 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth prime of the form 10*k^2+7, with k starting at 0
   (a(1)=7, a(2)=17, ...).  The function caches previously found primes
   so that calls in any order stay fast. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Test whether a small seq_t-valued integer is prime.
   Works for all supported seq_t types. */
static bool is_prime_small(seq_t num) {
    if (num < 2) return false;
    for (seq_t d = 2; d * d <= num; ++d) {
        if (num % d == 0) return false;
    }
    return true;
}

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

/* static cache of found primes */
static seq_t *prime_cache = NULL;
static idx_t  cache_len  = 0;
static idx_t  cache_cap  = 0;
static idx_t  next_k     = 0;

/* make sure the cache can hold at least 'need' elements */
static void ensure_cache_cap(idx_t need)
{
    if (need <= cache_cap) return;
    while (cache_cap < need)
        cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    prime_cache = mem_resize(seq_t, prime_cache, cache_cap);
    /* ignore realloc failure for brevity */
}

/* choose the appropriate primality test for the current seq_t size */
static inline bool is_prime_seq(seq_t v)
{
    if (sizeof(seq_t) <= sizeof(uint64_t))
        return is_prime_small((uint64_t)v);
    else
        return is_prime_big(v);
}

/* external function implementing the sequence */
seq_t a(idx_t n)
{
    if (n == 0)          /* out of offset – undefined, return 0 */
        return (seq_t)0;

    /* find more primes until we have at least n of them */
    while (cache_len < n) {
        seq_t k   = (seq_t)next_k;
        seq_t val = ((seq_t)10 * (k * k)) + (seq_t)7;

        if (is_prime_seq(val)) {
            ensure_cache_cap(cache_len + 1);
            prime_cache[cache_len] = val;
            ++cache_len;
        }
        ++next_k;
    }
    return prime_cache[n - 1];
}
