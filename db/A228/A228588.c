/*
 * A228588  Starting from a(1)=1, a(n) is the minimum integer greater than
 *          a(n-1) such that a(n)+a(n-1) and a(n)*a(n-1)+1 are both primes.
 * https://oeis.org/A228588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(1)=1, for n>1 a(n) is the least integer >a(n-1) such that both
 *            a(n)+a(n-1) and a(n)*a(n-1)+1 are prime
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 888 (2026-11-03)
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
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    if (n == 0)  /* undefined, return 0 to avoid UB */
        return (seq_t)0;

    /* initialise cache with a(1)=1 */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();
        cache[0] = (seq_t)1;  /* a(1) */
        cache_len = 1;
    }

    /* extend cache if required */
    if ((size_t)n > cache_len) {
        size_t target = (size_t)n;
        /* grow cache capacity (doubling) */
        while (cache_cap < target) cache_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
        if (!tmp) abort();
        cache = tmp;

        /* compute missing terms */
        for (size_t i = cache_len + 1; i <= target; ++i) {
            seq_t prev = cache[i - 2];          /* a(i-1) */
            seq_t cur  = prev + (seq_t)1;        /* start with opposite parity */

            /* search for the minimal admissible integer */
            for (;;) {
                seq_t sum = cur + prev;
                seq_t prod = cur * prev + (seq_t)1;

                if (is_prime_big(sum) && is_prime_big(prod))
                    break;
                cur += (seq_t)2;                /* keep parity */
            }
            cache[i - 1] = cur;  /* store a(i) */
        }
        cache_len = target;
    }

    return cache[n - 1];
}
