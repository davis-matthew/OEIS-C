/*
 * A045234  Numbers whose base-5 representation contains no 1's and exactly
 *          two 3's.
 * https://oeis.org/A045234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer whose base‑5 representation * contains
 *            no 1’s and exactly two 3’s
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* generated terms in ascending order   */
static size_t cache_len = 0;     /* number of terms stored               */
static size_t cache_cap = 0;     /* current capacity                      */
static idx_t  max_len_generated = 0; /* longest length that has been generated */

/* Append a value to the cache, expanding the array if necessary. */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* memory exhaustion */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Recursive generation of all numbers of a given length that fulfil the
 * digit constraints.  The parameters are:
 *   len   – total length of the number (in base‑5 digits)
 *   pos   – current position (0 … len‑1, 0 is most significant)
 *   cnt3  – how many 3’s have been placed so far
 *   val   – value already built from the preceding positions
 */
static void rec_generate(int len, int pos, int cnt3, seq_t val)
{
    if (pos == len) {                      /* all positions processed */
        if (cnt3 == 2)                     /* exactly two 3's required */
            append_cache(val);
        return;
    }

    if (pos == 0) {
        /* Leading digit may be 2, 3, or 4 (no zeros, no 1's). */
        const int first_digits[3] = {2, 3, 4};
        for (int i = 0; i < 3; ++i) {
            int d = first_digits[i];
            int new_cnt = cnt3 + (d == 3);
            if (new_cnt > 2) continue;
            seq_t new_val = val * 5 + d;   /* safe: overflow check below */
            if (new_val < val) continue;   /* overflow in unsigned arithmetic */
            rec_generate(len, pos + 1, new_cnt, new_val);
        }
    } else {
        /* Subsequent digits can be 0, 2, 3 or 4. */
        const int other_digits[4] = {0, 2, 3, 4};
        for (int i = 0; i < 4; ++i) {
            int d = other_digits[i];
            int new_cnt = cnt3 + (d == 3);
            if (new_cnt > 2) continue;
            seq_t new_val = val * 5 + d;
            if (new_val < val) continue;   /* skip overflowed branches */
            rec_generate(len, pos + 1, new_cnt, new_val);
        }
    }
}

/* Ensure that the cache contains at least n terms.  The function is
 * idempotent and safe for any call order. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;           /* already satisfied */

    /* Generate numbers for increasing lengths until enough terms are
       accumulated.  Each length is generated only once. */
    while (cache_len < n) {
        int len = (int)(max_len_generated + 1);   /* next length to generate */
        if (len >= 2)                               /* need at least two 3's */
            rec_generate(len, 0, 0, (seq_t)0);
        max_len_generated = len;
    }
}

/* External linkage required by the build system */
seq_t a(idx_t n)
{
    /* n is 1‑based; n ≥ 1 by the problem statement. */
    ensure_cache(n);
    return cache[n - 1];
}
