/*
 * A174625  Table T(n,k) with the coefficients of the polynomial P_n(x) =
 *          P_{n-1}(x) + x*P_{n-2}(x) + 1 in row n, by decreasing exponent of
 *          x.
 * https://oeis.org/A174625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A174625 – coefficients of the polynomials  
 *   P_1(x)=0, P_2(x)=2, P_n(x)=P_{n-1}(x)+x*P_{n-2}(x)+1   (n≥3). 
 * The terms are written row‑by‑row, from the highest exponent to the
 * constant term.  Hence a(1)=0, a(2)=2, a(3)=3, a(4)=2, a(5)=4, ….
 * 
 * Formula implemented: the recurrence above, with a(1)=0, a(2)=2, a(3)=3.
 * The function returns the n‑th term (1‑based index).  
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* static tables to avoid re‑computing rows on repeated calls */
    static seq_t *seq  = NULL;          /* flattened list of all terms */
    static size_t  seq_len = 0;         /* number of terms stored */
    static size_t  seq_cap = 0;         /* allocated capacity for seq */

    /* two previous polynomials, stored as coefficient arrays
     * with index = exponent (0 = constant term).  Their lengths are
     * floor((n-2)/2)+1 for row n, i.e. floor(row/2). */
    static seq_t *prevprev = NULL;      /* P_{row-2} */
    static seq_t *prev     = NULL;      /* P_{row-1} */
    static size_t prevprev_len = 0;     /* length of prevprev  */
    static size_t prev_len     = 0;     /* length of prev      */
    static idx_t next_row = 4;          /* index of the next row to generate */

    /* ------------------------------------------------------------------
     * Initialisation on the first call.
     * ------------------------------------------------------------------ */
    if (seq_len == 0) {
        /* initialise flattened list with the first three rows */
        seq_cap = 32;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        assert(seq != NULL);

        seq[0] = (seq_t)0;   /* P1 : 0            */
        seq[1] = (seq_t)2;   /* P2 : 2            */
        seq[2] = (seq_t)3;   /* P3 : 3            */
        seq_len = 3;

        /* initialise P2 and P3 for use in the recurrence */
        prevprev_len = 1;
        prevprev = (seq_t*)malloc(prevprev_len * sizeof(seq_t));
        assert(prevprev != NULL);
        prevprev[0] = (seq_t)2;                /* P2 */

        prev_len = 1;
        prev = (seq_t*)malloc(prev_len * sizeof(seq_t));
        assert(prev != NULL);
        prev[0] = (seq_t)3;                    /* P3 */

        next_row = 4;                          /* next row to generate */
    }

    /* ------------------------------------------------------------------
     * Grow the precomputed data until we have at least n terms.
     * ------------------------------------------------------------------ */
    while (seq_len < (size_t)n) {
        /* ----- 1. compute next polynomial P_next_row -------------------- */
        size_t curr_len = (size_t)(next_row / 2);     /* floor(next_row/2) */
        seq_t *curr = (seq_t*)malloc(curr_len * sizeof(seq_t));
        assert(curr != NULL);

        /* constant term   */
        curr[0] = prev[0] + (seq_t)1;

        /* other exponents */
        for (size_t k = 1; k < curr_len; ++k) {
            seq_t ck = 0;
            if (k < prev_len)          ck += prev[k];
            if (k - 1 < prevprev_len)  ck += prevprev[k - 1];
            curr[k] = ck;
        }

        /* ----- 2. append coefficients to the flat sequence (descending) */
        if (seq_len + curr_len > seq_cap) {
            size_t new_cap = seq_cap * 2;
            while (seq_len + curr_len > new_cap) new_cap *= 2;
            seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
            assert(seq != NULL);
            seq_cap = new_cap;
        }

        /* store in reverse order: highest exponent first */
        for (size_t k = curr_len; k > 0; --k) {
            seq[seq_len++] = curr[k - 1];
        }

        /* ----- 3. shift polynomials for next iteration ----------------- */
        seq_t *old_prevprev = prevprev;
        size_t old_prevprev_len = prevprev_len;

        prevprev = prev;
        prevprev_len = prev_len;

        prev = curr;
        prev_len = curr_len;

        /* free the earlier polynomial that is no longer needed */
        free(old_prevprev);

        /* advance to the next row number */
        ++next_row;
    }

    /* ------------------------------------------------------------------
     * Return the required term (1‑based index).
     * ------------------------------------------------------------------ */
    return seq[n - 1];
}
