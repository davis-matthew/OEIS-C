/*
 * A117076  Prime numbers with more even digits than odd digits.
 * https://oeis.org/A117076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 939 (2026-11-03)
 * Helpers:   digit_counts, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes with more even digits than odd digits (OEIS A117076). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_counts.h"
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 0 */

    /* Static cache of found terms. */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* Candidate to be tested for primality. */
    static seq_t next_candidate = (seq_t)2;
    static bool initialized = false;

    /* Ensure cache initialization only once (C++ static initialization). */
    if (!initialized) {
        initialized = true;
    }

    /* Grow cache as needed. */
    if (cache_size < n) {
        while (cache_size < n) {
            /* Find next prime starting from next_candidate. */
            seq_t cand = next_candidate;
            while (!is_prime_big(cand)) {
                cand = cand + (seq_t)1;
            }

            /* Count digits of the prime. */
            unsigned char cnt[10];
            digit_counts(cand, cnt);
            int even = cnt[0] + cnt[2] + cnt[4] + cnt[6] + cnt[8];
            int odd  = cnt[1] + cnt[3] + cnt[5] + cnt[7] + cnt[9];

            if (even > odd) {
                /* Store in cache. */
                if (cache_size + 1 > cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    cache = seq_realloc(cache, cache_cap, new_cap);
                    cache_cap = new_cap;
                }
                cache[cache_size++] = cand;
            }

            /* Set next_candidate to the immediate successor of the found prime. */
            next_candidate = cand + (seq_t)1;
        }
    }

    return cache[n - 1];
}
