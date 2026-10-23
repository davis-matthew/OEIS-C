/*
 * A026465  Length of n-th run of identical symbols in the Thue-Morse sequence
 *          A010060 (or A001285).
 * https://oeis.org/A026465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _grow_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A026465: length of the nth run of identical symbols
   in the Thue-Morse sequence (A010060).  The first run is the
   single 0 at n=0, so a(1)=1.  Subsequent runs are identified by
   changes in the Thue‑Morse bit value.  */

/* The function keeps a cached list of run lengths and generates as
   many runs as necessary on demand.  Since the average run length
   is 3/2, a call with n up to 10 000 requires only a few tens of
   thousands of Thue-Morse bits, well within the 2 s time budget. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_grow_cache.h"

seq_t a(idx_t n)
{
    /* static state used to memoise generated runs */
    static seq_t  *cache   = NULL;
    static size_t  cap     = 0;
    static size_t  size    = 0;      /* number of completed runs stored */
    static idx_t   next_bit = 0;      /* index of next Thue‑Morse bit to examine */
    static int     cur_bit = 0;       /* current run's symbol (0 or 1) */
    static size_t  cur_len  = 0;      /* length of the current (in‑progress) run */

    /* initialise on first call */
    if (size == 0 && cap == 0) {
        _grow_cache(&cache, &cap);
        cur_bit = __builtin_parityll(0);   /* t(0) = 0 */
        cur_len = 1;
        next_bit = 1;
    }

    /* generate runs until we have at least n runs cached */
    while (size < (size_t)n) {
        int bit = __builtin_parityll((unsigned long long)next_bit);
        if (bit == cur_bit) {
            /* same run continues */
            cur_len++;
            next_bit++;
        } else {
            /* run ended – store its length */
            if (size == cap) _grow_cache(&cache, &cap);
            cache[size++] = (seq_t)cur_len;
            /* start new run */
            cur_bit = bit;
            cur_len = 1;
            next_bit++;
        }
    }

    return cache[n - 1];
}
