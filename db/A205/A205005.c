/*
 * A205005  The triangular number T(j) such that n divides T(k)-T(j)>0, where
 *          k is the least positive integer for which such a j exists.
 * https://oeis.org/A205005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(j) where j is the first index whose triangular number
 *            shares a residue modulo n with a later triangular number.
 *            Equivalently, a(n) is the smallest T(j) such that there exists
 *            k>j with n | (T(k)-T(j)) and k is the least integer for which
 *            such a j exists
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used for n>=1 */
    /* Allocate array to store the first index that produced each residue. */
    idx_t *first = (idx_t*)calloc((size_t)n, sizeof(idx_t));
    if (!first) return 0; /* allocation failure fall‑back */

    /* Compute triangular numbers T(k) = k(k+1)/2 and look for a repeated residue. */
    seq_t tri = ((seq_t)1 * (seq_t)2) / 2;      /* T(1) */
    idx_t r = (idx_t)(tri % (seq_t)n);
    first[r] = 1;                               /* remember first occurrence */

    for (idx_t k = 2; ; ++k) {
        tri = ((seq_t)k * ((seq_t)k + 1)) / 2;
        r = (idx_t)(tri % (seq_t)n);
        if (first[r] != 0) {                    /* residue seen before at j */
            idx_t j = first[r];
            free(first);
            return ((seq_t)j * (seq_t)(j + 1)) / 2;
        }
        first[r] = k;
        /* By the pigeonhole principle a repeat must occur by k = n+1. */
    }
}
