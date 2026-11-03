/*
 * A155771  Numbers n such that 2*n^2+2*n-41 is a prime.
 * https://oeis.org/A155771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 83 terms through n = 872 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th integer m such that 2*m^2 + 2*m – 41 is prime.
   The implementation incrementally searches candidates; it caches found
   terms so that calls with arbitrary n order remain efficient. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
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

#ifdef __cplusplus
#include <vector>
#endif

/* Return the n-th term (n ≥ 1). */
seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++: use a vector to store the terms. */
    static std::vector<seq_t> cache;
    static idx_t next_candidate = 1;

    while (cache.size() < n) {
        seq_t val = 2 * (seq_t)next_candidate * (seq_t)next_candidate
                    + 2 * (seq_t)next_candidate - 41;
        if (val >= 2 && is_prime_big(val))
            cache.push_back((seq_t)next_candidate);
        ++next_candidate;
    }
    return cache[(size_t)(n - 1)];
#else
    /* C: use a realloc‑based dynamic array. */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0, cache_cap = 0;
    static idx_t next_candidate = 1;

    while (cache_sz < n) {
        seq_t val = 2 * (seq_t)next_candidate * (seq_t)next_candidate
                    + 2 * (seq_t)next_candidate - 41;
        if (val >= 2 && is_prime_big(val)) {
            if (cache_sz == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
#endif
}
