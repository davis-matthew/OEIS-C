/*
 * A181704  Numbers m=2^(t-1)*(2^t-5), where 2^t-5 is prime.
 * https://oeis.org/A181704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   is_prime_u64, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code computes A181704: a(n) = 2^(t-1) * (2^t-5) where t is the nth integer
 * such that 2^t-5 is prime. The function scans t upward, tests 
 * primality of 2^t-5 using is_prime_big, and memoizes found terms. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
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
    /* Static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* The smallest t that has been examined so far. t starts from 3. */
    static idx_t last_t = 2;

    /* Expand cache to accommodate n terms */
    while (cache_len < n) {
        /* Find next t that makes 2^t-5 prime */
        do {
            ++last_t;
            /* 2^t */
            seq_t pow_t = pow2(last_t);
            /* 2^t - 5 */
            seq_t pminus5 = pow_t - (seq_t)5;
            /* test primality */
            if (is_prime_big(pminus5)) {
                /* 2^(t-1) */
                seq_t pow_t_minus1 = pow2(last_t - 1);
                /* term = 2^(t-1) * (2^t - 5) */
                seq_t term = pow_t_minus1 * pminus5;

                /* Grow cache if necessary */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *new_cache = mem_new(seq_t, new_cap);
                    if (new_cache == NULL)
                        abort(); /* allocation failure */
                    if (cache)
                        mem_copy(new_cache, cache, cache_len * sizeof(seq_t));
                    mem_free(cache);
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = term;
                break;
            }
        } while (1);
    }

    return cache[n - 1];
}
