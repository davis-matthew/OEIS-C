/*
 * A175935  Number of lattice paths from (0,0) to (n,n) using steps
 *          S={(k,0),(0,k),(r,r)|0<k<=2,0<r<=2} which never go above the line
 *          y=x.
 * https://oeis.org/A175935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS A175935: 
   Number of lattice paths from (0,0) to (n,n) using steps 
   (1,0),(2,0),(0,1),(0,2),(1,1),(2,2) that never go above y=x.  
   The recurrence is
        f(0,0) = 1
        for i>=0, j>=0, j<=i
          f(i,j) = f(i-1,j) + f(i-2,j) + f(i,j-1) + f(i,j-2)
               + f(i-1,j-1) + f(i-2,j-2)
          (indices <0 are ignored).
   Return a(n) = f(n,n).  
*/

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Allocate temporary rows, cast to seq_t* so that code compiles in C++ */
    seq_t *prev      = mem_new(seq_t, 1);
    seq_t *prevprev  = mem_new(seq_t, 1);
    prev[0] = 1;          /* f(0,0) = 1 */
    prevprev[0] = 0;      /* dummy for missing row when i=1 */

    seq_t *curr = NULL;
    idx_t i, j;

    for (i = 1; i <= n; ++i) {
        size_t sz = (size_t)i + 1;   /* row length: j = 0 .. i */
        curr = mem_new(seq_t, sz);

        for (j = 0; j <= i; ++j) {
            seq_t val = 0;

            /* from (i-1,j) */
            if (j <= i-1) val += prev[j];

            /* from (i-2,j) */
            if (i >= 2 && j <= i-2) val += prevprev[j];

            /* from (i,j-1) */
            if (j >= 1) val += curr[j-1];

            /* from (i,j-2) */
            if (j >= 2) val += curr[j-2];

            /* from (i-1,j-1) */
            if (j >= 1) val += prev[j-1];

            /* from (i-2,j-2) */
            if (i >= 2 && j >= 2) val += prevprev[j-2];

            curr[j] = val;
        }

        /* rotate rows: prevprev <- prev, prev <- curr */
        seq_t *tmp = prevprev;
        prevprev = prev;
        prev    = curr;
        mem_free(tmp);
    }

    seq_t result = prev[n];
    mem_free(prevprev);
    mem_free(prev);
    return result;
}
