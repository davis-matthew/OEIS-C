/*
 * A186279  Numbers k such that the sum of the digits of k plus the sum of the
 *          digits of k^2 plus the sum of the digits of k^3 is prime.
 * https://oeis.org/A186279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 176, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9316 (2026-11-03)
 * Helpers:   digit_sum, ipow, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A186279:  a(n) = the nth integer k such that
    sum of digits of k + sum of digits of k^2 + sum of digits of k^3
    is a prime number. */
/*  Implementation: search all integers starting at 1 and
    store the discovered terms in a growing cache.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/ipow.h"
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
    /* static cache of found terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* last candidate checked */
    static idx_t last_k  = 0;

    /* number of terms currently stored */
    static idx_t cached_terms = 0;

    /* return cached value if already found */
    if (n <= cached_terms) {
        return cache[n - 1];
    }

    /* extend the cache until we have at least n terms */
    while (cached_terms < n) {
        /* next integer to test */
        ++last_k;
        seq_t k = (seq_t)last_k;

        /* compute digit sums of k, k^2 and k^3 */
        seq_t s1 = digit_sum(k);
        seq_t s2 = digit_sum(ipow(k, (seq_t)2));
        seq_t s3 = digit_sum(ipow(k, (seq_t)3));
        seq_t sum = s1 + s2 + s3;

        /* keep the term if the sum is prime */
        if (is_prime_big(sum)) {
            /* grow the cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {
                    /* allocation failed – abort */
                    exit(1);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
            ++cached_terms;
        }
    }

    return cache[n - 1];
}
