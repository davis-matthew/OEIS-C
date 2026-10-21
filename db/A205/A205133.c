/*
 * A205133  The number s(j) such that n divides s(k)-s(j), where
 *          s(j)=j*(2*j-1) and k is the least positive integer for which such
 *          a j exists.
 * https://oeis.org/A205133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A205133.
   For a given n ≥ 1, find the least j0 ≥ 1 such that
   there exists k > j0 with n | (s(k) – s(j0)),
   where s(j) = j * (2*j – 1).  The function returns s(j0).  */
/* This implementation uses a simple remainder‑table search
   which is O(n) time and O(n) space.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n is always ≥ 1, but guard against n = 0 to avoid UB. */
    if (n == 0)
        return (seq_t)0;

    /* Allocate an array of size n to store the first seen index
       for each remainder modulo n.  Index 0 is unused because
       j starts at 1, and we use 0 to mean “not seen yet”. */
    idx_t *seen = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!seen)   /* allocation failure – should never happen for n ≤ 10000 */
        return (seq_t)0;

    for (idx_t j = 1; ; ++j)
    {
        /* s(j) = j * (2*j – 1).  All arithmetic is performed
           with seq_t to avoid intermediate overflow of idx_t. */
        seq_t sj = (seq_t)j * ((seq_t)j * 2 - 1);

        /* Remainder modulo n.  Convert n to seq_t for the mod
           operation, then cast back to idx_t. */
        idx_t rem = (idx_t)(sj % (seq_t)n);

        /* If this remainder has appeared before, the difference
           of the corresponding s values is divisible by n.  The
           first occurrence is the desired j0. */
        if (seen[rem] != 0)
        {
            idx_t j0 = seen[rem];
            free(seen);
            /* Return s(j0).  Compute directly to avoid re‑computing. */
            return (seq_t)j0 * ((seq_t)j0 * 2 - 1);
        }

        /* Record the first occurrence of this remainder. */
        seen[rem] = j;
    }
}
