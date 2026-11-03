/*
 * A116533  a(1)=1, a(2)=2, for n > 2 if a(n-1) is prime, then a(n) =
 *          2*a(n-1), otherwise a(n) = a(n-1) - 1.
 * https://oeis.org/A116533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(2) = 2; for n > 2, a(n) = 2*a(n‑1) if a(n‑1) is
 *            prime, else a(n‑1)‑1
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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

static seq_t *cache = NULL;   /* 0‑based cache: cache[0] = a(1) */
static size_t cache_len = 0;  /* number of entries currently stored */
static size_t cache_cap = 0;  /* allocated capacity of cache */

/* Make sure cache has at least 'needed' slots. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    /* realloc failure leaves cache unchanged; caller will error on next use. */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                           /* invalid index – behaviour undefined */
        return (seq_t)0;

    /* Initialise base cases once */
    if (cache_len == 0) {
        ensure_cache(2);
        cache[0] = (seq_t)1;              /* a(1) */
        cache[1] = (seq_t)2;              /* a(2) */
        cache_len = 2;
    }

    /* Return cached value if already available */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Compute needed terms */
    size_t start = cache_len;             /* start index (0‑based) to compute */
    ensure_cache((size_t)n);
    for (size_t i = start; i < (size_t)n; ++i) {
        seq_t prev = cache[i - 1];        /* a(i) because cache[0] = a(1) */
        if (is_prime_big(prev))
            cache[i] = prev * (seq_t)2;   /* a(i+1) = 2*a(i) */
        else
            cache[i] = prev - (seq_t)1;   /* a(i+1) = a(i)‑1   */
    }
    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
