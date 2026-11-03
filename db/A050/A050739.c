/*
 * A050739  Numbers of form 9^k (values of k see A050730) containing no pair
 *          of consecutive equal digits (probably finite).
 * https://oeis.org/A050739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A050739: numbers 9^k (k from A050730) that contain no pair of consecutive equal digits. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    /* Cached results of the sequence. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t pow9 = 0;          /* current value of 9^k */
    static bool init = false;

    if (!init) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return 0;               /* out‑of‑memory – signal failure */
        cache_len = 0;
        pow9 = (seq_t)1;                   /* 9^0 = 1 */
        init = true;
    }

    /* Produce values until we have at least (n+1) entries. */
    while (cache_len <= n) {
        if (no_consecutive_equal_digits(pow9)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) { mem_free(cache); return 0; }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = pow9;
        }
        pow9 = mul_small(pow9, 9);
    }

    return cache[n];
}
