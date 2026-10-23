/*
 * A123976  Numbers k such that Fibonacci(k-1) is divisible by k.
 * https://oeis.org/A123976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that Fibonacci(k-1) is divisible by k
 * (OEIS A123976).  The algorithm tests successive k until it has
 * produced the n‑th element, keeping a cache so later calls are O(1). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Fast doubling implementation of Fibonacci(n) mod m, using 128‑bit
 * arithmetic to avoid overflow.  Returns Fn mod m. */
static inline uint64_t fib_mod_uint64(idx_t n, uint64_t mod)
{
    if (mod == 1) return 0;    /* every number is divisible by 1 */
    if (n == 0) return 0;      /* F(0) = 0 */

    uint64_t a = 0;            /* Fn  */
    uint64_t b = 1;            /* Fn+1 */
    int bits = 63 - __builtin_clzll(n);

    for (int i = bits; i >= 0; --i) {
        /* d = F(2k)   = F(k) * (2*F(k+1) - F(k))   */
        /* e = F(2k+1) = F(k)^2 + F(k+1)^2           */
        unsigned __int128 two_b = (unsigned __int128)b * 2ULL;
        uint64_t two_b_mod = (uint64_t)(two_b % mod);
        uint64_t two_b_minus_a =
            (two_b_mod >= a) ? two_b_mod - a
                              : two_b_mod + mod - a;

        unsigned __int128 mul_d = (unsigned __int128)a * two_b_minus_a;
        uint64_t d = (uint64_t)(mul_d % mod);

        unsigned __int128 mul_e =
            (unsigned __int128)a * a + (unsigned __int128)b * b;
        uint64_t e = (uint64_t)(mul_e % mod);

        if ((n >> i) & 1) {
            a = e;
            unsigned __int128 sum = (unsigned __int128)d + e;
            b = (uint64_t)(sum % mod);
        } else {
            a = d;
            b = e;
        }
    }
    return a;
}

/* Test whether k satisfies the property: Fibonacci(k-1) ≡ 0 (mod k). */
static inline bool good(uint64_t k)
{
    /* The implementation works for k >= 1; for k == 1 we return true
       because Fib(0)=0 is divisible by 1. */
    return fib_mod_uint64(k - 1, k) == 0;
}

/* ---- dynamic cache of found terms ---- */
static seq_t *cache        = NULL;   /* array of seq_t values */
static size_t cache_len    = 0;      /* how many terms currently cached */
static size_t cache_cap    = 0;      /* allocated capacity of the array */
static uint64_t last_seen  = 0;      /* largest k tested so far */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* ---- the sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but return 0 for safety */

    /* initialise the cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed */
        cache_cap = 4;
        cache_len = 0;
        last_seen = 0;
    }

    /* generate new terms until we have at least n terms cached */
    while (cache_len < (size_t)n) {
        ++last_seen;
        if (good(last_seen)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len] = (seq_t)last_seen;
            ++cache_len;
        }
    }

    return cache[n - 1];
}
