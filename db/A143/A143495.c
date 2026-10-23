/*
 * A143495  Triangle read by rows: 3-Stirling numbers of the second kind.
 * https://oeis.org/A143495
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns terms of OEIS A143495.
   The sequence is the triangle of 3‑Stirling numbers of the
   second kind (T(n,k)) read by rows.  For n ≥ 3 the term a(n)
   is T(m,k) where m and k are obtained from n by
     m = the smallest integer with (m−2)(m−1)/2 ≥ n−2,
     j = n − (m−3)(m−2)/2 − 1,   k = 3 + j.
   T(n,k) satisfies
     T(3,3) = 1,
     T(n,2) = 0 for all n,
     T(n,k) = T(n−1,k−1) + k * T(n−1,k)  for n>3, 3≤k≤n.
   The implementation generates rows of the triangle
   incrementally, caching all terms produced so far.
   It works for any seq_t type that supports ordinary
   arithmetic operators.  No floating‑point or external
   helpers are used. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* forward declarations of static variables */
    static seq_t *a_cache = NULL;
    static size_t a_cap   = 0;
    static size_t a_used  = 0; /* number of cached terms, a_cache[0..a_used-1] exist */

    static seq_t *prev = NULL;   /* previous row */
    static size_t prev_len = 0; /* length of prev (prev_len = row_index+1) */
    static size_t last_row = 2;   /* last computed row (row 2 has no data) */

    /* initialize cache on first call */
    if (a_cache == NULL) {
        /* reserve space for indices 0,1,2 (unused) */
        a_cap  = 8;
        a_used = 3;
        a_cache = (seq_t*)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = a_cache[1] = a_cache[2] = 0;

        /* build first row (n = 3) */
        prev_len = 4; /* indices 0..3 */
        prev = (seq_t*)malloc(prev_len * sizeof(seq_t));
        for (size_t i = 0; i < prev_len; ++i) prev[i] = 0;
        prev[3] = 1;             /* T(3,3) = 1 */
        last_row = 3;
        /* store its only term in the flattened sequence */
        if (a_used == a_cap) {
            a_cap *= 2;
            a_cache = (seq_t*)realloc(a_cache, a_cap * sizeof(seq_t));
        }
        a_cache[a_used++] = prev[3];   /* a(3) */
    }

    /* if requested index already cached */
    if ((size_t)n < a_used) return a_cache[(size_t)n];

    /* generate rows until we have term a(n) */
    while (a_used <= (size_t)n) {
        size_t next_row = last_row + 1;          /* new row number */
        size_t curr_len = next_row + 1;          /* array size needed */

        seq_t *curr = (seq_t*)malloc(curr_len * sizeof(seq_t));
        for (size_t i = 0; i < curr_len; ++i) curr[i] = 0;

        /* compute current row entries for k = 3..next_row */
        for (size_t k = 3; k <= next_row; ++k) {
            seq_t left  = (k-1 < prev_len) ? prev[k-1] : 0;
            seq_t mid   = (k   < prev_len) ? prev[k]   : 0;
            seq_t val   = left + (seq_t)k * mid;
            curr[k] = val;

            /* append this term to flattened cache */
            if (a_used == a_cap) {
                a_cap = (a_cap == 0) ? 8 : a_cap * 2;
                a_cache = (seq_t*)realloc(a_cache, a_cap * sizeof(seq_t));
            }
            a_cache[a_used++] = val;
        }

        /* discard previous row, move to current */
        free(prev);
        prev = curr;
        prev_len = curr_len;
        last_row = next_row;
    }

    return a_cache[(size_t)n];
}
