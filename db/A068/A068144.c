/*
 * A068144  Triangular numbers which are a concatenation of two or more
 *          positive triangular numbers.
 * https://oeis.org/A068144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 480, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   append_candidate, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A068144:  Triangular numbers which are concatenations of two or more
   positive triangular numbers.  The function generates triangular numbers
   T = m(m+1)/2 in increasing order, tests whether the decimal
   representation of T can be split into at least two substrings that are
   themselves triangular numbers, and stores the qualifying T in a
   static cache.  The nth element of the cache is returned as a(idx_t n).   */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#include "../utils/floor_sqrt.h"
#include "../utils/append_candidate.h"

/* Helper: test if a triangular number n satisfies 8*n+1 is a perfect
   square with an odd sqrt, i.e. n = t(t+1)/2 for some integer t.           */
static bool is_tri(seq_t n)
{
    seq_t d = n * (seq_t)8 + 1;          /* 1 + 8*n                      */
    seq_t s = floor_sqrt(d);             /* floor sqrt(1+8*n)            */
    if (s * s != d) return false;        /* not a perfect square          */
    /* s must be odd so that (s-1)/2 is integer.                      */
    return ((s - 1) & 1) == 0;
}

/* Helper: determine whether T (already known to be triangular) can be
   written as a concatenation of two or more positive triangular numbers. */
static bool is_concatenation(seq_t T)
{
    if (T == 0) return false;                     /* exclude zero */

    /* Convert T to its decimal digit array (most‑significant first).   */
    size_t len = 0;
    seq_t v = T;
    while (v > 0) { v /= 10; len++; }

    int *dig = (int*)malloc(len * sizeof(int));
    if (!dig) return false;
    v = T;
    for (size_t i = 0; i < len; i++) {
        dig[len - 1 - i] = (int)(v % (seq_t)10);
        v /= 10;
    }

    bool *dp = (bool*)calloc(len + 1, sizeof(bool));
    if (!dp) { free(dig); return false; }
    dp[0] = true;

    for (size_t i = 0; i < len; i++) {
        if (!dp[i]) continue;
        if (dig[i] == 0) continue;          /* leading zero not allowed */

        seq_t val = 0;
        for (size_t j = i; j < len; j++) {
            val = val * 10 + (seq_t)dig[j];
            if (is_tri(val)) {
                if (j == len - 1 && i > 0) {   /* two or more parts */
                    free(dig);
                    free(dp);
                    return true;
                }
                dp[j + 1] = true;
            }
        }
    }

    free(dig);
    free(dp);
    return false;
}

/* Static cache for the pre‑computed terms of the sequence.             */
static seq_t *cache = NULL;      /* array of found values          */
static size_t cache_size = 0;    /* number of cached terms         */
static size_t cache_cap  = 0;    /* allocated capacity              */
static idx_t tested_m = 1;       /* next triangular index to test   */

/* Expand the cache until it contains at least n_needed terms.           */
static void ensure_cache(idx_t n_needed)
{
    /* Determine the maximum representable value for seq_t.               */
    const size_t bits = sizeof(seq_t) * CHAR_BIT;
    __uint128_t max_val;
    if (bits < 128) {
        max_val = (((__uint128_t)1) << bits) - 1;
    } else {
        max_val = ~((__uint128_t)0);
    }

    bool overflowed = false;

    while (cache_size < (size_t)n_needed && !overflowed) {
        __uint128_t t128 = (__uint128_t)tested_m * (tested_m + 1) / 2;
        if (t128 > max_val) {            /* would not fit in seq_t       */
            overflowed = true;
            break;
        }
        seq_t T = (seq_t)t128;

        if (is_concatenation(T))
            append_candidate(&cache, &cache_size, &cache_cap, T);

        tested_m++;
    }
}

/* Public function required by the framework.                            */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety, offset is 1     */
    ensure_cache(n);
    return cache[n - 1];
}
