/*
 * A050734  Numbers of form 4^k (values of k see A050725) containing no pair
 *          of consecutive equal digits (probably finite).
 * https://oeis.org/A050734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 4^k where k is the n-th exponent such that 4^k has no *
 *            consecutive equal decimal digits
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include <stdint.h>
#include <string.h>
#include <math.h>
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

static seq_t *cached_terms = NULL;    /* dynamic array of found terms   */
static size_t cache_capacity = 0;      /* current array capacity         */
static idx_t term_count = 0;           /* number of terms stored         */
static seq_t current_val = 0;          /* 4^current_k                    */
static idx_t current_k = 0;            /* exponent of current_val         */

/* Ensure the cache has at least 'needed' slots. */
static void ensure_capacity(size_t needed)
{
    if (cache_capacity < needed) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 4;
        while (new_cap < needed)
            new_cap *= 2;

        seq_t *tmp = seq_alloc_new_array(new_cap);
        if (!tmp) abort();                /* out of memory */
        for (size_t i = 0; i < cache_capacity; ++i)
            tmp[i] = cached_terms[i];
        if (cached_terms)
            seq_free_array(cached_terms);
        cached_terms = tmp;
        cache_capacity = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n < 1 */

    /* Generate terms until we have at least n of them. */
    while (term_count < n) {
        seq_t val;
        if (current_k == 0 && current_val == 0) {
            /* 4^0 = 1 */
            val = (seq_t)1;
        } else {
            val = mul_small(current_val, (unsigned int)4);
        }

        if (no_consecutive_equal_digits(val)) {
            ensure_capacity(term_count + 1);
            cached_terms[term_count++] = val;
        }

        current_val = val;
        current_k++;
    }

    return cached_terms[n - 1];
}
