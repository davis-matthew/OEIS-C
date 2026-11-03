/*
 * A215523  Slowest increasing sequence of alternate-parity integers m such
 *          that 2m+1 is prime.
 * https://oeis.org/A215523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 960 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: A215523 is defined by starting with m(1)=1 and then, for
 * each n>1, choosing the smallest integer m>m(n-1) of opposite parity
 * such that 2m+1 is prime.  This implementation builds the sequence
 * incrementally, caching previously computed values to support arbitrary
 * call order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

static seq_t *cache = NULL;          /* 1‑based cache for computed terms */
static size_t cache_cap = 0;         /* allocated capacity of the cache   */
static idx_t  cache_len = 0;         /* largest index already computed */

/* Ensure the cache has room for at least 'needed' entries (index 1‑based). */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for index 0 (offset 1) */

    /* Return cached value if already known. */
    if (n <= cache_len)
        return cache[n];

    /* Extend the cache as far as needed. */
    grow_cache((size_t)n + 1);               /* allow 1‑based indexing */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t m;
        if (i == 1) {
            m = (seq_t)1;                    /* a(1) = 1 */
        } else {
            seq_t last = cache[cache_len];
            seq_t cand = last + (seq_t)1;    /* start at next integer */

            /* Ensure the parity alternates. */
            if ((cand & (seq_t)1) == (last & (seq_t)1))
                cand += (seq_t)1;

            /* Find the next m of the desired parity such that 2m+1 is prime. */
            for (;;) {
                seq_t p = cand + cand + (seq_t)1;   /* 2*cand + 1, no overflow in addition */
                if (is_prime_big(p)) {
                    m = cand;
                    break;
                }
                cand += (seq_t)2;                  /* keep parity */
            }
        }
        cache[i] = m;
        cache_len = i;
    }
    return cache[n];
}
