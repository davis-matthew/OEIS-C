/*
 * A066121  Multi-level factorials: triangle with a(n,k)=a(n-1,k-1)*a(n-1,k)
 *          but with a(n,1)=n and a(n,n)=1.
 * https://oeis.org/A066121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 30, u32 n <= 38, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A066121: flattened multi‑level factorial triangle
   a(n) is obtained by reading the triangle
   with a(n,k) = a(n-1,k-1) * a(n-1,k),
   a(n,1) = n, a(n,n) = 1. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *prev = NULL;   /* previous row values */
    static idx_t  prev_len = 0;  /* length of previous row */

    static seq_t *flatten = NULL; /* flattened triangle values */
    static size_t  flen = 0, fcap = 0;

    static idx_t row_no = 1; /* next row number to generate */

    /* Grow until we have at least n values */
    while (flen < n)
    {
        size_t L = (size_t)row_no;           /* length of new row    */
        seq_t *curr = (seq_t*)malloc(L * sizeof(seq_t)); /* new row buffer      */
        if (!curr) return (seq_t)0;           /* allocation failed    */

        if (row_no == 1)          /* first row          */
        {
            curr[0] = (seq_t)1;
        }
        else
        {
            curr[0]   = (seq_t)row_no;          /* left edge */
            curr[L-1] = (seq_t)1;              /* right edge */
            for (idx_t k = 1; k + 1 < L; ++k)   /* interior cells */
                curr[k] = prev[k-1] * prev[k];
        }

        /* Append the new row to the flattened array */
        for (idx_t j = 0; j < L; ++j)
        {
            if (flen == fcap)                      /* grow flatten if needed */
            {
                size_t new_cap = fcap == 0 ? 8 : fcap * 2;
                seq_t *tmp = (seq_t*)realloc(flatten, new_cap * sizeof(seq_t));
                if (!tmp) { free(curr); return (seq_t)0; }
                flatten = tmp;
                fcap    = new_cap;
            }
            flatten[flen++] = curr[j];
        }

        /* Prepare for next iteration: free old previous row
           and use the new row as the previous row. */
        if (prev) free(prev);
        prev      = curr;
        prev_len  = L;

        ++row_no;     /* advance to next row number */
    }

    return flatten[n - 1];
}
