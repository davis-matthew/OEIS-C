/*
 * A044151  Numbers k such that string 1,6 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1446, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044151: Numbers k such that the base‑7 representation of
   k contains the substring "16" while the representation of k‑1 does
   not.  The function `a(n)` returns the n‑th such number (offset 1). */

#include <stdbool.h>
#include <stdlib.h>

/* Return true iff the base‑7 representation of x contains the
   consecutive digits 1 followed by 6.  */
static bool has16(seq_t x)
{
    /* Scan digits from least‑significant to most‑significant. */
    bool first = true;
    seq_t prev = 0;                      /* previous digit (LSB‑1) */
    while (x > 0) {
        seq_t d = x % 7;                 /* current digit (LSB) */
        x /= 7;
        if (!first && prev == 6 && d == 1)
            return true;                /* found 1,6 in the correct order */
        prev = d;
        first = false;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* ---------- cache design ------------------------------------ */
    /* Static state keeps the computation incremental and
       independent of call order. */
    static seq_t *cache      = NULL;   /* array of found numbers */
    static size_t  cap       = 0;      /* current allocation size     */
    static idx_t   cached_cnt = 0;     /* number of terms stored      */
    static seq_t   next_k     = 1;     /* next integer to test        */
    static bool    prev_has   = false; /* has16(next_k - 1)            */

    /* Return from cache when possible. */
    if (n <= cached_cnt)
        return cache[n - 1];

    /* Grow cache if necessary. */
    if (cap == 0) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (cached_cnt < n) {
        bool cur_has = has16(next_k);
        if (cur_has && !prev_has) {          /* new occurrence */
            if (cached_cnt >= cap) {          /* extend cache */
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[cached_cnt++] = next_k;
        }
        prev_has = cur_has;
        next_k++;
    }

    return cache[n - 1];
}
