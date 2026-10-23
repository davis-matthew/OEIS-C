/*
 * A118966  a(n) = (n+1)/2 if n occurs among the first n-1 terms of the
 *          sequence, otherwise a(n) = 2*n - 1.
 * https://oeis.org/A118966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)/2 if n occurs among the first n-1 terms, otherwise
 *            a(n) = 2*n-1. Sequence index starts at 1
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Cached sequence values: seq[i] == a(i+1).  seq_len stores the number of terms
   currently computed.  'used' records which positive integers have appeared
   as values; used[v] == 1 iff v has already been used.  Because a(n)
   (except when returned (n+1)/2) is at most 2*n-1, we only need to
   remember values up to 2*max_n-1. */
static seq_t *seq   = NULL;
static size_t seq_len = 0;          /* number of terms already computed */

static unsigned char *used = NULL;
static size_t used_cap = 0;          /* size of the 'used' array */

/* Ensure the sequence has been generated up to index n (1‑based). */
static void ensure_sequence(idx_t n)
{
    if (seq_len >= (size_t)n)          /* already up to date */
        return;

    /* Allocate space for the values that will be written. */
    size_t old_len = seq_len;
    seq = (seq_t *)realloc(seq, (size_t)n * sizeof(seq_t));
    if (!seq) abort();                 /* memory error */

    /* Ensure 'used' can index up to 2*n‑1.  The capacity is at least 2*n
       so that indices 0 … 2*n‑1 are safe. */
    size_t new_used_cap = 2 * (size_t)n;
    if (used_cap < new_used_cap) {
        unsigned char *new_used = (unsigned char *)realloc(used, new_used_cap);
        if (!new_used) abort();       /* memory error */
        /* Zero the freshly allocated part. */
        memset(new_used + used_cap, 0,
               (new_used_cap - used_cap) * sizeof(unsigned char));
        used = new_used;
        used_cap = new_used_cap;
    }

    /* Build the missing terms.  A term depends only on whether its index
       has already appeared as a value. */
    for (size_t k = old_len; k < (size_t)n; ++k) {
        idx_t idx = (idx_t)(k + 1);        /* current sequence index (1‑based) */
        seq_t val;

        if (used[(size_t)idx]) {          /* the index has appeared before */
            val = ((seq_t)(idx + 1)) / 2; /* (n+1)/2  */
        } else {                          /* first occurrence of idx */
            val = ((seq_t)2 * (seq_t)idx) - 1; /* 2*n-1 */
        }

        seq[k] = val;                      /* store the value */

        /* Mark the value as used.  The value is at most 2*idx-1 <= 2*n-1,
           and used_cap ≥ 2*n, so the index is in bounds. */
        size_t v_idx = (size_t)(idx_t)val;  /* convert to a size_t index */
        used[v_idx] = 1;
    }
    seq_len = n;
}

/* Public entry point.  Works for any order of calls and any n ≥ 1. */
seq_t a(idx_t n)
{
    ensure_sequence(n);
    return seq[n - 1];
}
