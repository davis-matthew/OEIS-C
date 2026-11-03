/*
 * A071868  Number of integers k (1 <= k <= n) such that k^2+1 is prime.
 * https://oeis.org/A071868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = # { k | 1 ≤ k ≤ n and k^2 + 1 is prime }
 * Range:     u8 n <= 15, u16 n <= 418, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9038 (2026-11-03)
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

seq_t a(idx_t n)
{
    /* Ignore zero or negative indices – the OEIS definition starts at 1. */
    if (n == 0) return 0;

    /* Persistent cache of prefix counts: cache[i] holds a(i+1). */
    static seq_t *cache = NULL;        /* cache[0] = a(1) etc. */
    static idx_t cached_n = 0;         /* largest n already cached */

    /* Return cached result if we already have it. */
    if (n <= cached_n) return cache[n - 1];

    /* Resize cache to hold up to n; use malloc/realloc so it works
       in both C and C++. */
    seq_t *new_cache = mem_resize(seq_t, cache, (size_t)n);
    if (!new_cache) return 0;          /* allocation failure – give up */
    cache = new_cache;

    /* Current accumulator equals the last cached value, or 0 if nothing
       has been cached yet. */
    seq_t acc = (cached_n == 0) ? 0 : cache[cached_n - 1];

    /* Compute new terms for k = cached_n+1 … n. */
    for (idx_t k = cached_n + 1; k <= n; ++k)
    {
        /* Compute k^2 + 1 using seq_t to avoid overflow. */
        seq_t val = (seq_t)k * (seq_t)k + (seq_t)1;

        /* Count it if k^2 + 1 is prime. */
        if (is_prime_big(val)) ++acc;

        cache[k - 1] = acc;
    }

    cached_n = n;
    return cache[n - 1];
}
