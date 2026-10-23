/*
 * A044386  Numbers n such that string 5,4 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044386 – numbers n such that the decimal representation of n
   contains the substring “54” but the decimal representation of n‑1 does not.
   The implementation scans the integers, checks the property and caches
   the found terms. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_digits.h"

static seq_t *cache = NULL;      /* cached sequence terms            */
static size_t cache_len = 0;      /* number of cached terms           */
static size_t cache_cap = 0;      /* capacity of the cache array      */
static seq_t curr = 0;            /* last integer examined            */

/* Check whether the decimal digits (left‑to‑right) contain the substring
   “54”.  get_digits stores digits in reverse order (least‑significant
   first), so the pattern “54” corresponds to “45” in that array. */
static bool has54(seq_t v)
{
    size_t len;
    uint8_t buf[64];             /* 64 digits is enough for 128‑bit numbers */
    get_digits(v, buf, &len);

    for (size_t i = 0; i + 1 < len; ++i)
        if (buf[i] == 4 && buf[i + 1] == 5)   /* reversed pair “45” */
            return true;
    return false;
}

/* Return the n‑th term of the sequence (n ≥ 1).  The function is
   order‑independent; it maintains a cache of previously found terms.
   If the cache does not contain enough terms yet it continues to
   scan integers until the required number of terms has been found. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but harmless */

    /* Grow the cache until we have at least n terms. */
    if (cache_len < (size_t)n) {
        /* Initialise the cache on first use. */
        if (!cache) {
            cache_cap = 128;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) exit(1);            /* allocation failure */
        }

        while (cache_len < (size_t)n) {
            ++curr;                          /* next integer to test */

            if (!has54(curr)) continue;       /* must contain “54” */

            /* The preceding integer must NOT contain “54”. */
            if (has54(curr - 1)) continue;

            /* Store the found term. */
            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(1);          /* reallocation failure */
            }
            cache[cache_len++] = curr;
        }
    }

    return cache[(size_t)n - 1];
}
