/*
 * A038678  Squares ending in a different nonzero square.
 * https://oeis.org/A038678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 208, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth square whose decimal representation ends with a
   non‑zero square suffix (a proper suffix – the suffix must be
   shorter than the whole number).  The sequence is OEIS A038678. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"
#include "../utils/digit_len.h"

/* Return true if sq has a proper suffix that is a non‑zero perfect square. */
static bool qualifies(seq_t sq)
{
    size_t digits = digit_len(sq);
    /* suffix length must be at least 1 and less than the whole length */
    for (size_t k = 1; k < digits; ++k) {
        seq_t mod = sq % pow10((idx_t)k);   /* last k digits */
        if (mod == 0) continue;            /* suffix must be non‑zero */
        seq_t r   = floor_sqrt(mod);
        if (r * r == mod) return true;      /* suffix is a perfect square */
    }
    return false;
}

/* Cache of found sequence terms. */
static seq_t  *cache       = NULL;
static size_t  cache_size  = 0;
static size_t  cache_count = 0;
static idx_t   next_root   = 1;   /* next integer to test */

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined offset */

    /* Extend cache until we have at least n terms. */
    while (cache_count < (size_t)n) {
        seq_t sq = ((seq_t)next_root) * next_root;  /* current square */
        ++next_root;

        if (qualifies(sq)) {
            if (cache_count >= cache_size) {
                size_t new_size = cache_size ? cache_size * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
                if (!tmp) return (seq_t)0;            /* allocation failed */
                cache       = tmp;
                cache_size  = new_size;
            }
            cache[cache_count++] = sq;
        }
    }

    return cache[(size_t)n - 1];
}
