/*
 * A186075  Irregular triangle, read by rows, of primes with suffix n and
 *          digits "9" prepended, otherwise 0.
 * https://oeis.org/A186075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A186075: 
 * For each row number r (starting at 1) generate numbers 
 *   L(r,k) = (repunit_k)*10^d + r          where d = digits(r) and repunit_k = 99…9 (k digits).
 * Emit all L(r,k) that are prime, stopping at the first composite L(r,k).  
 * If a row produces no primes, output a single 0 for that row.  
 * The sequence is read by rows, concatenating the outputs of all rows.
 */

#include <stdbool.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/num_digits_idx.h"
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

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

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_row = 1;           /* row number to be processed next */

    /* Grow the cache when needed */
    while (cache_len < n) {
        idx_t r = next_row++;
        /* Compute base = 10^digits(r) */
        int d = num_digits_idx(r);
        seq_t base = (seq_t)1;
        for (int i = 0; i < d; ++i) base *= (seq_t)10;

        /* repunit = 9, then repeat: repunit = repunit*10 + 9 */
        seq_t repunit = (seq_t)9;
        seq_t first = repunit * base + (seq_t)r;

        bool any_prime = false;
        if (is_prime_big(first)) {
            /* Ensure capacity */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = first;
            any_prime = true;
        } else {
            /* No prime in this row -> output a single 0 */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)0;
            continue;   /* move to next row */
        }

        /* Continue for k >= 2 until first composite */
        while (true) {
            repunit = repunit * (seq_t)10 + (seq_t)9;
            seq_t val = repunit * base + (seq_t)r;
            if (is_prime_big(val)) {
                if (cache_len + 1 > cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = seq_realloc(cache, cache_cap, new_cap);
                    cache_cap = new_cap;
                }
                cache[cache_len++] = val;
                any_prime = true;
                continue;
            } else {
                /* Stop at first composite; do not output zero for it */
                break;
            }
        }
        /* If the row produced primes, we do not output an extra zero. */
    }

    return cache[n - 1];
}
