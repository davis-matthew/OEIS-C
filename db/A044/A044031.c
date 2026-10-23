/*
 * A044031  Numbers k such that 5 and 6 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 5 and 6 occur juxtaposed in the decimal representation
 * of k but not of k+1.  This function returns the n‑th such number, with
 * a(1) being the first term listed in the OEIS data.  The implementation
 * searches numbers sequentially, memorising results so that later calls
 * with larger n are fast. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return true if x contains the substring "56" or "65".  Works for any
 * unsigned integer type seq_t. */
static bool has56or65(seq_t x)
{
    seq_t prev = x % (seq_t)10;
    x    /= (seq_t)10;
    while (x > (seq_t)0) {
        seq_t cur = x % (seq_t)10;
        if ((cur == (seq_t)5 && prev == (seq_t)6) ||
            (cur == (seq_t)6 && prev == (seq_t)5))
            return true;
        prev = cur;
        x    /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Constant capacity for an empty cache (will grow as needed). */
    static size_t cache_cap   = 0;
    static seq_t *cache       = NULL;
    static size_t cache_len   = 0;      /* number of terms cached so far */
    static seq_t next_k       = (seq_t)1; /* next integer to examine */

    if (cache_cap == 0) {              /* initialise cache */
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) {                  /* allocation failed; abort */
            abort();
        }
    }

    /* Keep searching until we have at least n terms cached. */
    while (cache_len < n) {
        if (has56or65(next_k) && !has56or65(next_k + (seq_t)1)) {
            if (cache_len == cache_cap) {          /* grow cache */
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              sizeof(seq_t) * new_cap);
                if (!tmp) abort();                  /* realloc failed */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = next_k;
        }
        next_k++;                                   /* examine next integer */
    }

    return cache[n - 1];
}
