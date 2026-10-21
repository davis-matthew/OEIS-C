/*
 * A077367  Triangular numbers whose external digits form a triangular number.
 *          Or triangular number whose MSD and LSD form a triangular number.
 * https://oeis.org/A077367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A077367: Triangular numbers whose most significant and least significant
   decimal digits form a triangular number.  The function computes the
   n‑th triangular number T = k*(k+1)/2 such that the two‑digit number
   formed by the first and last digit of T is triangular.  Values are
   generated on demand and cached so that calls in any order are
   efficient. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utilities that are available in the build environment */
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

/* ------------------------------------------------------------------ */
/* Static cache and state                                            */
/* ------------------------------------------------------------------ */
static seq_t *cache      = NULL;   /* Cached triangular numbers satisfying the condition */
static size_t cache_len  = 0;      /* Number of cached terms */
static size_t cache_cap  = 0;      /* Allocated capacity of cache */
static idx_t  next_k     = 0;      /* Next triangular index to test */

/* ------------------------------------------------------------------ */
/* Returns the number of decimal digits of |x| (x is non‑negative).  */
/* ------------------------------------------------------------------ */
static size_t digit_count_seq(seq_t x)
{
    size_t len = 1;
    while (x >= 10) {
        x /= 10;
        ++len;
    }
    return len;
}

/* ------------------------------------------------------------------ */
/* Tests whether n is a triangular number  (8*n+1 is a perfect square). */
/* ------------------------------------------------------------------ */
static bool is_triangular(seq_t n)
{
    seq_t y = 8 * n + 1;
    seq_t r = floor_sqrt(y);
    return r * r == y;
}

/* ------------------------------------------------------------------ */
/* Grows the cache array to accommodate at least one more element.    */
/* ------------------------------------------------------------------ */
static void grow_cache(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort(); /* fatal error */
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Ensures that at least n terms are present in the cache.            */
/* ------------------------------------------------------------------ */
static void ensure_cached(idx_t n)
{
    while (cache_len < n) {
        seq_t k_val = (seq_t)next_k;
        seq_t T     = k_val * (k_val + 1) / 2;     /* T_k */

        /* Determine external two‑digit number */
        size_t len   = digit_count_seq(T);
        seq_t last   = T % 10;
        seq_t first;
        if (len == 1) {
            first = T; /* single digit */
        } else {
            seq_t pow  = pow10(len - 1); /* 10^(len-1) */
            first = T / pow;
        }
        seq_t ext = (len == 1) ? T : (first * 10 + last);

        /* Test the condition and cache if satisfied */
        if (is_triangular(ext)) {
            grow_cache();
            cache[cache_len++] = T;
        }

        ++next_k;
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function.                                          */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0)                /* undefined below offset; safe to return 0 */
        return (seq_t)0;
    ensure_cached(n);
    return cache[n - 1];
}
