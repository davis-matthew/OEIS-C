/*
 * A044007  Numbers k such that 1 and 4 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1261, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_14_or_41
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 1 and 4 occur juxtaposed in the base‑10 representation
   of k but not of k+1.  This function is memoized and computes the sequence on demand. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_14_or_41.h"

/* Sequence cache and state. */
static seq_t *s_cache    = NULL;   /* cached sequence values */
static idx_t  s_len      = 0;      /* number of terms currently stored */
static idx_t  s_cap      = 0;      /* allocated capacity of s_cache */
static seq_t  s_last_k   = 0;      /* last computed k, so next candidate is s_last_k + 1 */

/* Compute the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;   /* undefined for n==0; return 0 as defensive value */

    /* Extend cache until it contains at least n terms. */
    while (s_len < n) {
        /* Find the next k satisfying the definition. */
        seq_t k = s_last_k + (seq_t)1;
        for (;;) {
            if (contains_14_or_41(k) && !contains_14_or_41(k + (seq_t)1)) {
                /* Append k to cache. */
                if (s_len == s_cap) {
                    idx_t new_cap = (s_cap == (idx_t)0) ? (idx_t)8 : s_cap * (idx_t)2;
                    seq_t *new_arr = (seq_t*)realloc(s_cache, new_cap * sizeof(seq_t));
                    s_cache = new_arr;
                    s_cap   = new_cap;
                }
                s_cache[s_len++] = k;
                s_last_k = k;
                break;
            }
            k = k + (seq_t)1;
        }
    }
    return s_cache[n - (idx_t)1];
}
