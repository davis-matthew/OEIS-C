/*
 * A015922  Numbers k such that 2^k == 8 (mod k).
 * https://oeis.org/A015922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k such that 2^k == 8 (mod k)
 * Range:     u8 n <= 30, u16 n <= 2554, u32 n <= 29055, u64 n <= 29055,
 *            u128 n <= 29055, big any n
 * Validated: standard, pass, 72 terms through n = 9830 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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
        seq_t rem = powmod((seq_t)2, next_k, kst); /* 2^k mod k */
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
