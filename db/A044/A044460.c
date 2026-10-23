/*
 * A044460  Numbers n such that string 0,2 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 2929, u32 n <= 3512, u64 n <= 3512,
 *            u128 n <= 3512, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function returns the nth number n (with 1‑based indexing) such
    that the base‑5 representation of n contains the substring “02”
    whereas the base‑5 representation of n+1 does not.
    The implementation performs a linear search and caches the
    discovered values so that subsequent calls are efficient. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;       /* cached sequence values */
static size_t cache_len = 0;      /* number of cached values */
static size_t cache_cap = 0;      /* current capacity of cache array */
static seq_t last_checked = 0;    /* last integer examined during search */

/* Return true iff x (in base 5) contains the substring “02”. */
static bool has_02(seq_t x)
{
    /* for x < 5 the base‑5 string has length 1 and cannot contain “02”. */
    if (x < 5) return false;

    /* Extract base‑5 digits into an array (least‑significant first). */
    int digits[70];          /* 70 > log5(2^128) for 128‑bit types */
    int d = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        digits[d++] = (int)(tmp % 5);
        tmp /= 5;
    }

    /* Look for the pattern 0 followed by 2 in the high‑to‑low order. */
    for (int i = d - 1; i > 0; --i) {
        if (digits[i] == 0 && digits[i - 1] == 2) return true;
    }
    return false;
}

/* Append a new value to the cache, enlarging it if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);     /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 1 */

    /* Convert the 64‑bit index to a size_t.  The required range is far
       below SIZE_MAX for the intended use (up to thousands). */
    size_t target = (size_t)n;

    /* Incrementally search until we have enough entries cached. */
    while (cache_len < target) {
        seq_t cand = last_checked + 1;
        last_checked = cand;

        if (has_02(cand) && !has_02(cand + 1)) {
            cache_append(cand);
        }
    }

    return cache[n - 1];
}
