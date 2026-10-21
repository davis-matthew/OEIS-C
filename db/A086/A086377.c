/*
 * A086377  a(1)=1; a(n)=a(n-1)+2 if n is in the sequence; a(n)=a(n-1)+2 if n
 *          and (n-1) are not in the sequence; a(n)=a(n-1)+3 if n is not in
 *          the sequence but (n-1) is in the sequence.
 * https://oeis.org/A086377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence-based computation of A086377.
   The sequence is defined by a(1)=1 and for n>1:

   a(n) = a(n‑1) + 2   if n is a term of the sequence or both
                          n and n‑1 are not terms,
   a(n) = a(n‑1) + 3   if n is not a term but n‑1 is a term.

   The function keeps a memoised array of values and a boolean table
   recording which integers have already appeared as terms. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *a_arr = NULL;      /* array of terms, 1‑based       */
    static idx_t  a_len = 0;         /* greatest index that has been computed */
    static bool   *seen   = NULL;    /* table of integers that are terms */
    static idx_t  seen_cap = 0;      /* capacity of seen[]               */

    /* -------------------------------------------------------------
       Allocation / reallocation of a_arr
       ------------------------------------------------------------- */
    if (!a_arr) {
        a_arr = (seq_t *)malloc((n + 2) * sizeof(seq_t));
        if (!a_arr) return (seq_t)0; /* out of memory */
        a_len = 0;
    } else if (n + 1 > (idx_t)( (sizeof(seq_t) * (size_t)0) ? 0 : 0)) {
        /* grow a_arr if requested n is larger than current capacity */
        seq_t *tmp = (seq_t *)realloc(a_arr, (n + 2) * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* out of memory */
        a_arr = tmp;
    }

    /* -------------------------------------------------------------
       Allocation / reallocation of seen
       ------------------------------------------------------------- */
    if (!seen) {
        /* allow indices up to n; later also need space for terms that
           may exceed n (which are < 3*n for the intended n range). */
        seen_cap = n + 10;
        seen = (bool *)calloc((size_t)seen_cap, sizeof(bool));
        if (!seen) return (seq_t)0; /* out of memory */
    } else if (n + 1 > seen_cap) {
        size_t new_cap = seen_cap;
        while ((size_t)(n + 1) > new_cap) new_cap *= 2;
        bool *tmp = (bool *)realloc(seen, new_cap * sizeof(bool));
        if (!tmp) return (seq_t)0; /* out of memory */
        /* initialise the new part */
        memset(tmp + seen_cap, 0, (new_cap - seen_cap) * sizeof(bool));
        seen     = tmp;
        seen_cap = new_cap;
    }

    /* -------------------------------------------------------------
       Base case: a(1)=1
       ------------------------------------------------------------- */
    if (a_len == 0) {
        a_arr[1] = (seq_t)1;
        a_len   = 1;
        /* ensure seen[1] exists */
        if (1 >= seen_cap) {
            bool *tmp = (bool *)realloc(seen, (seen_cap * 2) * sizeof(bool));
            if (!tmp) return (seq_t)0;
            memset(tmp + seen_cap, 0, (seen_cap) * sizeof(bool));
            seen = tmp;
            seen_cap *= 2;
        }
        seen[1] = true;
    }

    /* -------------------------------------------------------------
       Compute terms up to index n
       ------------------------------------------------------------- */
    for (idx_t i = a_len + 1; i <= n; ++i) {
        bool prev_in_seq = seen[i - 1];
        bool cur_in_seq  = (i < seen_cap) ? seen[i] : false;

        seq_t inc = (seq_t)2;
        if (!cur_in_seq && prev_in_seq)
            inc = (seq_t)3;

        a_arr[i] = a_arr[i - 1] + inc;

        /* Record that a[i] is now a term */
        seq_t val = a_arr[i];
        idx_t val_idx = (idx_t)val;          /* assumption: val fits in idx_t */

        /* grow seen if necessary to index val_idx */
        if (val_idx >= seen_cap) {
            size_t new_cap = seen_cap;
            while ((size_t)val_idx >= new_cap) new_cap *= 2;
            bool *tmp = (bool *)realloc(seen, new_cap * sizeof(bool));
            if (!tmp) return (seq_t)0;       /* out of memory */
            memset(tmp + seen_cap, 0, (new_cap - seen_cap) * sizeof(bool));
            seen = tmp;
            seen_cap = new_cap;
        }
        seen[val_idx] = true;
    }

    a_len = n;
    return a_arr[n];
}
