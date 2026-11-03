/*
 * A033875  Skipping from prime to prime by least powers of 2.
 * https://oeis.org/A033875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(n+1) = a(n) + 2^k where k minimal such that a(n)+2^k
 *            is prime
 * Range:     u8 n <= 18, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: standard, pass, 23 terms through n = 23 (2026-11-03)
 * Helpers:   is_prime_u64, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"
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

seq_t a(idx_t n) {
    static seq_t *cache = NULL;           /* cache[0] holds a(1) */
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Convert idx_t to size_t for array indexing; safe for n <= 10000 */
    size_t target = (size_t)n;
    if (target == 0) return (seq_t)0; /* undefined, but avoid crash */

    /* Grow cache if necessary */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = mem_resize(seq_t, cache, cache_cap);
        cache_len = 1;
        cache[0] = (seq_t)2;               /* a(1) = 2 */
    }

    while (cache_len < target) {
        seq_t cur = cache[cache_len - 1];
        seq_t nxt = 0;
        for (idx_t k = 0;; ++k) {
            seq_t candidate = cur + pow2(k);
            if (is_prime_big(candidate)) {
                nxt = candidate;
                break;
            }
        }
        /* Grow array if needed */
        if (cache_len == cache_cap) {
            cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_len++] = nxt;
    }

    return cache[target - 1];
}
