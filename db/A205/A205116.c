/*
 * A205116  Least s(k) such that n divides s(k)-s(j) for some j<k, where
 *          s(j)=is the j-th Lucas number.
 * https://oeis.org/A205116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least Lucas number that repeats a residue modulo n
 * Range:     u8 n <= 26, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for calloc, free */

/* The build supplies the types seq_t and idx_t.  idx_t is an unsigned
   integer type (uint64_t).  seq_t can be an unsigned integer
   (uint8_t … uint64_t) or an arbitrary-precision integer class. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but safe */

    /* Allocate table of first occurrences of residues modulo n.
       Residues range from 0 to n-1. */
    idx_t *first = (idx_t*)calloc((size_t)n, sizeof(*first));
    if (!first)      /* allocation failure */
        return 0;

    /* Lucas numbers: L1 = 1, L2 = 3.  Maintain both the full value
       and its value modulo n. */
    seq_t L_prev  = (seq_t)1;   /* L1   */
    seq_t L_curr  = (seq_t)3;   /* L2   */
    idx_t r_prev  = (idx_t)(1 % n);
    idx_t r_curr  = (idx_t)(3 % n);

    /* Index k corresponds to the Lucas index (1‑based). */
    idx_t k = 1;
    first[r_prev] = k;   /* record first occurrence of residue of L1 */

    k = 2;
    if (first[r_curr] != 0) {   /* residue repeats immediately */
        free(first);
        return L_curr;          /* L2 is the answer for this n */
    }
    first[r_curr] = k;

    /* Generate successive Lucas numbers until a residue repeats. */
    while (1) {
        seq_t L_next = L_prev + L_curr;
        idx_t r_next = (idx_t)((r_prev + r_curr) % n);

        ++k;                /* index of L_next */

        if (first[r_next] != 0) {
            /* First repeat: return the full Lucas number. */
            free(first);
            return L_next;
        }
        first[r_next] = k;

        /* Prepare for next iteration. */
        L_prev = L_curr;
        L_curr = L_next;
        r_prev = r_curr;
        r_curr = r_next;
    }
}
