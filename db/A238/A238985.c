/*
 * A238985  Zeroless 7-smooth numbers.
 * https://oeis.org/A238985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 245, u32 n <= 1162, u64 n <= 4382,
 *            u128 n <= 9785, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Zeroless 7‑smooth numbers:
   a(n) = the n‑th 7‑smooth number that contains no digit 0.
   A 4‑pointer Hamming‑number generator is used, and every generated
   number is tested for a zero digit before being stored in the memoized
   zeroless list. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */
#include <stdbool.h>  /* bool */

static inline bool has_no_zero(seq_t v)
{
    seq_t t = v;
    if (t == (seq_t)0)     /* zero never occurs, but keep for completeness */
        return true;
    while (t > (seq_t)0) {
        if (t % (seq_t)10 == (seq_t)0)
            return false;
        t /= (seq_t)10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* Static state – generated 7‑smooth numbers and zeroless subsequence. */
    /* ------------------------------------------------------------------ */
    static seq_t *smooth      = NULL;   /* all 7‑smooth numbers */
    static size_t smooth_len  = 0;
    static size_t smooth_cap  = 0;

    static idx_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;   /* 4 pointers */

    static seq_t *zeroless   = NULL;   /* zeroless 7‑smooth numbers */
    static size_t zeroless_len = 0;
    static size_t zeroless_cap = 0;

    /* ------------------------------------------------------------------ */
    /* Lazy initialisation                                                     */
    /* ------------------------------------------------------------------ */
    if (smooth == NULL) {
        smooth_cap   = 64;
        smooth       = (seq_t *)malloc(smooth_cap * sizeof(seq_t));
        smooth[0]    = (seq_t)1;
        smooth_len   = 1;

        zeroless_cap = 64;
        zeroless     = (seq_t *)malloc(zeroless_cap * sizeof(seq_t));
        zeroless[0]  = (seq_t)1;
        zeroless_len = 1;
    }

    /* ------------------------------------------------------------------ */
    /* Generate until we have the required zeroless term                  */
    /* ------------------------------------------------------------------ */
    while (zeroless_len < (size_t)n) {
        /* Next candidates multiplied by 2,3,5,7 */
        seq_t next2 = smooth[i2] * (seq_t)2;
        seq_t next3 = smooth[i3] * (seq_t)3;
        seq_t next5 = smooth[i5] * (seq_t)5;
        seq_t next7 = smooth[i7] * (seq_t)7;

        /* Choose the smallest candidate */
        seq_t next = next2;
        if (next3 < next) next = next3;
        if (next5 < next) next = next5;
        if (next7 < next) next = next7;

        /* Append to the smooth list, growing capacity if required */
        if (smooth_len == smooth_cap) {
            smooth_cap *= 2;
            smooth = (seq_t *)realloc(smooth, smooth_cap * sizeof(seq_t));
        }
        smooth[smooth_len++] = next;

        /* Advance any pointers that matched the chosen value */
        if (next2 == next) ++i2;
        if (next3 == next) ++i3;
        if (next5 == next) ++i5;
        if (next7 == next) ++i7;

        /* Test for zeroless property */
        if (has_no_zero(next)) {
            /* Append to zeroless list, growing if necessary */
            if (zeroless_len == zeroless_cap) {
                zeroless_cap *= 2;
                zeroless = (seq_t *)realloc(zeroless, zeroless_cap * sizeof(seq_t));
            }
            zeroless[zeroless_len++] = next;
        }
    }

    /* ------------------------------------------------------------------ */
    /* Return requested term (1‑based index).                              */
    /* ------------------------------------------------------------------ */
    return zeroless[(size_t)(n - 1)];
}
