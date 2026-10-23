/*
 * A045602  Numbers k that divide 9^k + 6^k.
 * https://oeis.org/A045602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   mod_pow, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the n‑th integer k with k | (9^k + 6^k).
   The function searches progressively for such k, storing found
   values in a static cache so repeated calls are efficient. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mod_pow.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

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
        seq_t p9 = mod_pow((seq_t)9, k, modk);
        seq_t p6 = mod_pow((seq_t)6, k, modk);
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
