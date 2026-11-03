/*
 * A050738  Numbers of form 8^k (values of k see A050729) containing no pair
 *          of consecutive equal digits (probably finite).
 * https://oeis.org/A050738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the value 8^k where k is the n‑th exponent such that
 * 8^k has no pair of consecutive equal decimal digits (see sequence A050729).
 * The function generates these values on demand, caching them for later use.
 *
 * The implementation relies on the utility functions:
 *   - no_consecutive_equal_digits(seq_t x)  – tests for consecutive equal digits.
 *   - mul_small(seq_t x, unsigned int m)    – multiplies a big integer by a small int.
 *   - grow_seq_array(seq_t **arr, size_t *cap, size_t new_len) – resizes the array safely.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Test whether a seq_t value has any pair of consecutive equal decimal digits. */
static inline bool no_consecutive_equal_digits(seq_t val) {
    if (val == (seq_t)0) return true;          /* 0 contains only one digit */
    int prev = -1;                             /* previous digit, -1 means none yet */
    seq_t tmp = val;
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);        /* current least‑significant digit */
        if (prev == d) return false;           /* found equal neighbours */
        prev = d;
        tmp /= (seq_t)10;                      /* shift right one decimal digit */
    }
    return true;                               /* all adjacent digits differ */
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of terms stored */

    /* current power of 8 being tested (8^current_k) */
    static seq_t power = (seq_t)1; /* 8^0 */
    static idx_t current_k = 0;    /* current exponent (kept for clarity) */

    /* generate terms until we have at least n+1 entries */
    while (cache_len <= n) {
        if (no_consecutive_equal_digits(power)) {
            /* ensure enough capacity */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                grow_seq_array(&cache, &cache_cap, new_cap);
            }
            cache[cache_len++] = power;
        }
        /* move to next power of 8 */
        ++current_k;
        power = mul_small(power, (unsigned int)8);
    }

    return cache[n];
}
