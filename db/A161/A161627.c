/*
 * A161627  Positions n such that A010060(n)=A010060(n+4).
 * https://oeis.org/A161627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Positions n such that the Thue–Morse sequence satisfies A010060(n)=A010060(n+4). */
/* Property: n has bit 2 set and the run of consecutive 1‑bits starting at that
   bit has odd length. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache of the sequence values. */
    static seq_t *vals = NULL;
    static size_t len = 0;   /* number of elements stored   */
    static size_t cap = 0;   /* allocated capacity          */
    static idx_t next = 1;   /* next integer to test        */

    /* Grow the cache until we have at least n elements. */
    while (len < n) {
        if (next & (idx_t)4) {                /* bit 2 must be 1          */
            idx_t y = next >> 2;              /* start at bit 2           */
            bool odd = false;
            while (y & (idx_t)1) {            /* count consecutive 1‑bits */
                odd = !odd;
                y >>= 1;
            }
            if (odd) {                        /* odd run length ⇒ accepted */
                if (len == cap) {             /* need more space          */
                    size_t newcap = cap ? cap * 2 : 8;
                    seq_t *tmp = seq_realloc(vals, cap, newcap);
                    if (!tmp) return 0;       /* allocation failed        */
                    vals = tmp;
                    cap = newcap;
                }
                vals[len++] = (seq_t)next;     /* store the value          */
            }
        }
        ++next;                               /* check the next integer   */
    }
    return vals[n - 1];                       /* 1‑based indexing         */
}
