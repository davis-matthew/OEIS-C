/*
 * A045602  Numbers k that divide 9^k + 6^k.
 * https://oeis.org/A045602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the n‑th integer k with k | (9^k + 6^k).
   The function searches progressively for such k, storing found
   values in a static cache so repeated calls are efficient. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
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
    /* static state for memoisation */
    static seq_t *terms   = NULL;   /* stored sequence values */
    static size_t terms_cap = 0;    /* allocated capacity of terms */
    static idx_t  terms_len  = 0;    /* number of terms stored */
    static idx_t  tested_upto = 0;  /* largest k that has been tested */

    /* handle calls below the offset safely */
    if (n == 0) return 0;

    /* initialise cache on first call */
    if (!terms) {
        terms_cap = 16;
        terms = seq_alloc(terms_cap);
    }

    /* keep generating terms until we have at least n of them */
    while (terms_len < n) {
        idx_t k = tested_upto + 1;          /* next integer to test */
        tested_upto = k;

        seq_t modk = (seq_t)k;
        seq_t p9 = powmod((seq_t)9, k, modk);
        seq_t p6 = powmod((seq_t)6, k, modk);
        seq_t sum = p9 + p6;
        sum %= modk;

        if (sum == (seq_t)0) {              /* k satisfies the property */
            /* enlarge cache if necessary */
            if (terms_len >= terms_cap) {
                size_t new_cap = terms_cap * 2 + 1;
                terms = seq_realloc(terms, terms_cap, new_cap);
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)k;
        }
    }
    return terms[n - 1];
}
