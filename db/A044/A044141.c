/*
 * A044141  Numbers k such that string 0,3 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1507, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044141: Numbers k whose base‑7 representation contains
   the substring "0,3" while k-1 does not. The routine builds the
   sequence incrementally, caching results so that calls can arrive in
   any order. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Internal cache data */
static seq_t   *a_cache      = NULL;
static size_t   a_cache_len  = 0;
static size_t   a_cache_cap  = 0;
static seq_t   a_last_val   = 0;      /* value of the last examined integer */
static bool     a_prev_has  = false;  /* whether a_last_val - 1 has "03" */

/* Determines whether the base‑7 representation of x contains the substring "0,3". */
static bool has_03(seq_t x)
{
    if (x == 0) return false;     /* base‑7 representation of 0 is just "0" */
    uint8_t digits[64];           /* 64 > log_7(2^128) */
    size_t len = 0;
    seq_t t = x;

    /* Extract digits least‑significant first */
    while (t) {
        digits[len++] = (uint8_t)(t % 7);
        t = t / 7;
    }

    /* Scan from most‑significant to least‑significant */
    for (size_t i = len; i > 1; --i) {
        if (digits[i-1] == 0 && digits[i-2] == 3)
            return true;
    }
    return false;
}

/* Fills the cache until it contains at least `needed` terms. */
static void ensure_cache(size_t needed)
{
    if (!a_cache) {
        a_cache_cap = 128;
        a_cache = (seq_t*)malloc(a_cache_cap * sizeof(seq_t));
        a_cache_len = 0;
        a_last_val = 0;
        a_prev_has = false;
    }

    while (a_cache_len < needed) {
        a_last_val += 1;
        bool cur_has = has_03(a_last_val);

        if (cur_has && !a_prev_has) {
            if (a_cache_len >= a_cache_cap) {
                a_cache_cap *= 2;
                a_cache = (seq_t*)realloc(a_cache, a_cache_cap * sizeof(seq_t));
            }
            a_cache[a_cache_len++] = a_last_val;
        }
        a_prev_has = cur_has;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined offset, but safe */
    ensure_cache((size_t)n);
    return a_cache[(size_t)n - 1];
}
