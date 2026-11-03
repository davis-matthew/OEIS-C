/*
 * A173092  Numbers k such that 3k-4, 3k-2, 3k+2, and 3k+4 are primes.
 * https://oeis.org/A173092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th integer k such that 3k-4, 3k-2, 3k+2, 3k+4 are all prime. */

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
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static seq_t next_k = 3;
    static bool initialized = false;

    if (!initialized) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)3;   /* a(1) = 3 */
        cache_len = 1;
        next_k = 5;            /* start with 5, odd numbers only */
        initialized = true;
    }

    while (cache_len < n) {
        /* only odd k > 3 */
        seq_t k = next_k;
        next_k += 2;

        seq_t t  = (seq_t)3 * k;   /* 3k */
        seq_t p1 = t - 4;
        seq_t p2 = t - 2;
        seq_t p3 = t + 2;
        seq_t p4 = t + 4;

        if (is_prime_big(p1) && is_prime_big(p2) &&
            is_prime_big(p3) && is_prime_big(p4)) {

            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp) return (seq_t)0;
                cache = tmp;
            }
            cache[cache_len++] = k;
        }
    }

    return cache[n - 1];
}
