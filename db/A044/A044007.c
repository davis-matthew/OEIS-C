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
 * Validated: standard, pass, 57 terms through n = 9496 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 1 and 4 occur juxtaposed in the base‑10 representation
   of k but not of k+1.  This function is memoized and computes the sequence on demand. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether decimal representation of x contains the substrings "14" or "41". */
static bool contains_14_or_41(seq_t x)
{
    /* Need at least two digits to contain a juxtaposition. */
    if (x < (seq_t)10)
        return false;

    seq_t prev = x % (seq_t)10;   /* least significant digit */
    x = x / (seq_t)10;

    while (x > (seq_t)0) {
        seq_t curr = x % (seq_t)10;
        if ((curr == (seq_t)1 && prev == (seq_t)4) ||
            (curr == (seq_t)4 && prev == (seq_t)1))
            return true;
        prev = curr;
        x = x / (seq_t)10;
    }
    return false;
}

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
                    seq_t *new_arr = mem_resize(seq_t, s_cache, new_cap);
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
