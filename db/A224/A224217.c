/*
 * A224217  Numbers b such that b^2 + 4*b + 9 is prime.
 * https://oeis.org/A224217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A224217: a(n) is the nth integer b for which
   b^2 + 4*b + 9 is prime. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif
#include <stdbool.h>
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
    if (n == 0) {
        /* offset 1 – safety fallback */
        return 0;
    }

#ifdef __cplusplus
    static std::vector<seq_t> cache_vec;
    static idx_t next_b = 1;

    while (cache_vec.size() < (size_t)n) {
        seq_t b = (seq_t)next_b;
        seq_t f = b * b + (seq_t)4 * b + (seq_t)9;
        if (is_prime_big(f)) {
            cache_vec.push_back(b);
        }
        ++next_b;
    }

    return cache_vec[n - 1];
#else
    static seq_t *cache_ptr = NULL;
    static size_t sz = 0;
    static size_t cap = 0;
    static idx_t next_b = 1;

    while (sz < (size_t)n) {
        seq_t b = (seq_t)next_b;
        seq_t f = b * b + (seq_t)4 * b + (seq_t)9;
        if (is_prime_big(f)) {
            if (cap == sz) {
                size_t new_cap = cap ? cap << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(cache_ptr, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache_ptr = tmp;
                cap = new_cap;
            }
            cache_ptr[sz++] = b;
        }
        ++next_b;
    }

    return cache_ptr[n - 1];
#endif
}
