/*
 * A045204  Numbers whose base-5 representation contains no 0's and exactly
 *          two 4's.
 * https://oeis.org/A045204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of base‑5 numbers with no 0's and exactly two 4's (A045204) */

#include <stdlib.h>

/* cache for generated terms */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;      /* capacity of cache */
static idx_t cache_len = 0;       /* number of terms stored */

/* state for successive generations */
static idx_t next_len = 2;        /* next digit length to generate */
static idx_t cur_len  = 0;        /* current digit length being generated */

/* Recursive generator for a fixed length cur_len.
   pos      – current digit position (0 … cur_len-1)
   rem4     – number of 4's still to place
   val      – current value built so far */
static void rec(idx_t pos, int rem4, seq_t val)
{
    if (pos == cur_len) {                /* all positions filled */
        if (rem4 == 0) {                 /* exactly two 4's used */
            if (cache_len >= cache_cap) { /* grow cache if necessary */
                cache_cap = (cache_cap == 0) ? 16384 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = val;     /* store the term */
        }
        return;
    }

    idx_t remaining = cur_len - pos;      /* positions left to fill */
    if ((idx_t)rem4 > remaining) return; /* not enough spots for remaining 4's */

    for (int d = 1; d <= 4; ++d) {
        if (d == 4) {
            if (rem4 == 0) continue;      /* cannot place more 4's */
            rec(pos + 1, rem4 - 1, val * 5 + d);
        } else {
            rec(pos + 1, rem4, val * 5 + d);
        }
    }
}

/* Return the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 0, harmless */

    /* initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 16384;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        cur_len = next_len;
        rec(0, 2, (seq_t)0);            /* start with two 4's to place */
        ++next_len;
    }
    return cache[n - 1];
}
