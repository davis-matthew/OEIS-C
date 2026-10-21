/*
 * A090270  Least positive r such that concatenation k, r, k is prime, where k
 *          == 1,3,7 or 9 (mod 10) in increasing order.
 * https://oeis.org/A090270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A090270
   a(n) = least positive r such that the concatenation k·r·k is prime,
          where k runs over the integers ≡1,3,7,9 (mod 10) in increasing order. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* number of decimal digits of a non‑zero uint64_t */
static inline int digit_len_uint64(uint64_t v)
{
    int d = 0;
    do { ++d; v /= 10U; } while (v);
    return d;
}

/* 10^e for e ≤ 19 (fits in uint64_t) */
static inline uint64_t pow10_uint64(uint32_t e)
{
    uint64_t v = 1;
    while (e--) v *= 10ULL;
    return v;
}

/* sequence implementation ---------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* allocated length */
    static size_t cache_cnt  = 0;      /* number of terms stored */
    static uint64_t next_k = 1;        /* next k to test (has allowed last digit) */

    if (n == 0) return (seq_t)0; /* offset is 1; guard */

    /* initialise cache if needed */
    if (!cache) {
        cache_size = 16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */
    }

    /* extend cache until we have the required term */
    while (cache_cnt < (size_t)n) {
        /* find the next k with last digit 1,3,7,9 */
        uint64_t k = next_k;
        while (true) {
            uint8_t d = k % 10U;
            if (d == 1 || d == 3 || d == 7 || d == 9) break;
            ++k;
        }
        next_k = k + 1;          /* next search starts after this k */

        int len_k = digit_len_uint64(k);
        uint64_t pow10_k = pow10_uint64((uint32_t)len_k);

        /* find least r such that concat(k,r,k) is prime */
        uint64_t r;
        for (r = 1; ; ++r) {
            int len_r = digit_len_uint64(r);
            uint64_t pow10_k_r = pow10_uint64((uint32_t)(len_k + len_r));

            /* candidate = k * 10^(len_k+len_r) + r * 10^len_k + k */
            /* compute using 128‑bit to avoid overflow on intermediate */
            __uint128_t cand128 = (__uint128_t)k * pow10_k_r
                                + (__uint128_t)r * pow10_k
                                + k;

            if (cand128 > UINT64_MAX)
                continue; /* skip too large, unlikely for required n */

            uint64_t cand = (uint64_t)cand128;

            if (is_prime(cand)) {
                /* store r in cache */
                if (cache_cnt >= cache_size) {
                    /* grow cache */
                    size_t new_size = cache_size * 2;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_size * sizeof(seq_t));
                    if (!tmp) return (seq_t)0; /* realloc failed */
                    cache = tmp;
                    cache_size = new_size;
                }
                cache[cache_cnt++] = (seq_t)r;
                break;
            }
        }
    }

    return cache[n - 1];
}
