/*
 * A205562  Least positive integer j such that n divides (2k)!-(2j)!, where k,
 *          as in A205561, is the least number for which there is such a j.
 * https://oeis.org/A205562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm finds, for given n≥1, the minimum j>0 such that
 * n | (2k)! - (2j)! where k is the smallest integer for which such a
 * j exists.  We compute the sequence of (2m)! mod n for m = 1,2,… .
 * The first time a residue repeats we have found that minimal k=m and
 * the earlier m' that produced the same residue is the desired j.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* n≥1 is intended */

    /* Array that stores the first occurrence (index) of each residue;
     * 0 means “not seen yet”.  The size is n because residues are in
     * [0 .. n-1].  One cast is required for C++ compatibility. */
    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!first) return 0;                  /* allocation failure */

    seq_t fac = (seq_t)(2 % (seq_t)n);      /* (2·1)! mod n = 2! mod n */
    idx_t m   = 1;                          /* current m */

    while (1) {
        /* Convert the residue to a small integer index for the array */
        idx_t idx = (idx_t)fac;
        if (first[(size_t)idx] == 0) {
            first[(size_t)idx] = m;         /* remember first occurrence */
        } else {
            idx_t j = first[(size_t)idx];   /* first occurrence is j */
            free(first);
            return (seq_t)j;                /* answer for a(n) */
        }

        /* Prepare next factorial: multiply by (2m+1) and (2m+2) */
        ++m;
        idx_t t1 = 2 * m - 1;                /* odd multiplier */
        idx_t t2 = 2 * m;                    /* even multiplier */

        fac = (fac * (seq_t)t1) % (seq_t)n;
        fac = (fac * (seq_t)t2) % (seq_t)n;
    }
}
