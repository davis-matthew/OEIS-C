/*
 * A044917  Numbers whose base-16 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A044917 by brute‑force search: checks each integer for alternating
   run lengths in base‑16 starting with an odd run length. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether `num` has alternating run lengths in base‑16
   (first run must be odd, next even, next odd, …). */
static bool alternates_hex(seq_t num)
{
    seq_t tmp = num;
    /* Collect hex digits least‑significant first. */
    int digits[1024];
    size_t len = 0;

    if (tmp == (seq_t)0) {
        digits[len++] = 0;
    } else {
        while (tmp > (seq_t)0) {
            digits[len++] = (int)(tmp % (seq_t)16);
            tmp /= (seq_t)16;
        }
    }

    /* Process digits most‑significant first. */
    int expected = 1; /* 1 = odd, 0 = even; first run must be odd. */
    int prev = digits[len - 1];
    size_t run = 1;

    for (long long i = (long long)len - 2; i >= 0; --i) {
        int d = digits[i];
        if (d == prev) {
            ++run;
        } else {
            if ((run & 1) != expected)
                return false;
            expected ^= 1;           /* toggle between odd/even */
            prev = d;
            run = 1;
        }
    }
    /* Final run satisfies the same parity rule. */
    return (run & 1) == expected;
}

/* Static cache of found terms */
static seq_t *cache = NULL;
static size_t cache_len   = 0;   /* number of cached terms   */
static size_t cache_cap   = 0;   /* allocated capacity       */
static idx_t  last_checked = 0;  /* last integer examined    */

/* Ensure the cache can hold at least `needed` elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp)
            abort();           /* Out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)            /* No a(0) in OEIS; safety fallback. */
        return (seq_t)0;

    ensure_cache_capacity(n);
    while (cache_len < (size_t)n) {
        ++last_checked;
        seq_t cand = (seq_t)last_checked;
        if (alternates_hex(cand)) {
            cache[cache_len++] = cand;
        }
    }
    return cache[(size_t)n - 1];
}
