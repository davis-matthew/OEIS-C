/*
 * A015922  Numbers k such that 2^k == 8 (mod k).
 * https://oeis.org/A015922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k such that 2^k == 8 (mod k)
 * Range:     u8 n <= 30, u16 n <= 2554, u32 n <= 9830, u64 n <= 9830,
 *            u128 n <= 9830, big any n
 * Helpers:   mod_pow, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* offset is 1 – n >= 1 is assumed */
    static seq_t *cache  = NULL;      /* stored terms */
    static size_t  clen   = 0;        /* number of terms cached */
    static size_t  cap    = 0;        /* allocated capacity */
    static idx_t   next_k = 1;        /* next k to test */

    if (n == 0) return 0; /* undefined for n<1 */

    /* Grow cache until we have at least n terms */
    while (clen < (size_t)n) {
        seq_t kst = (seq_t)next_k;          /* k as seq_t for calculations */
        seq_t rem = mod_pow((seq_t)2, next_k, kst); /* 2^k mod k */
        seq_t tgt = (seq_t)8 % kst;         /* 8 mod k */

        if (rem == tgt) {
            if (cache == NULL) {
                /* initial allocation */
                cap = 16;
                cache = seq_alloc(cap);
                if (cache == NULL) abort();
            } else if (clen == cap) {
                /* double capacity */
                size_t new_cap = cap * 2;
                seq_t *tmp = seq_realloc(cache, cap, new_cap);
                if (tmp == NULL) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[clen++] = next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
