/*
 * A032848  Numbers whose base-10 representation Sum_{i=0..m} d(i)*10^i has
 *          d(m) < d(m-1) > d(m-2) < ...
 * https://oeis.org/A032848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 8651, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc_guarded
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A032848 – Numbers whose decimal digits alternate.
    A(n) is the n‑th natural number for which the digits of the
    decimal representation satisfy
        d(m) < d(m-1) > d(m-2) < d(m-3) > … ,
    i.e. starting from the most significant digit the comparison
    alternates between “<” and “>”.  */

#include <stddef.h>   /* size_t, NULL, idx_t, seq_t */
#include <stdbool.h>  /* bool */
#include <stdlib.h>   /* malloc, realloc, free */
#include "../utils/seq_realloc_guarded.h"

static bool is_alternating(seq_t x)
{
    /* Single digit numbers satisfy the property. */
    if (x < 10) return true;

    /* Extract digits into a temporary array, MSD first. */
    int rev[128];          /* enough for 128‑bit integers */
    int len = 0;
    seq_t t = x;
    while (t != 0) {
        unsigned int d = (unsigned int)(t % 10);
        rev[len++] = d;
        t /= 10;
    }
    /* Reverse to obtain MSD first */
    for (int i = 0; i < len / 2; ++i) {
        int tmp = rev[i];
        rev[i] = rev[len - 1 - i];
        rev[len - 1 - i] = tmp;
    }

    bool less = true; /* first comparison is "<" */
    for (int i = 1; i < len; ++i) {
        if (less) {
            if (rev[i - 1] >= rev[i]) return false;
        } else {
            if (rev[i - 1] <= rev[i]) return false;
        }
        less = !less;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static seq_t next_candidate = 1;

    while (cache_size < n) {
        seq_t cand = next_candidate;
        if (is_alternating(cand)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)seq_realloc_guarded(cache, cache_cap, new_cap, sizeof(seq_t));
                if (!tmp) return 0; /* out of memory; return 0 */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
        next_candidate = cand + 1;
    }

    return cache[n - 1];
}
