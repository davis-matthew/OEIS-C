/*
 * A232094  a(n) = A060130(A000217(n)); number of nonzero digits in factorial
 *          base representation (A007623) of 0+1+2+...+n.
 * https://oeis.org/A232094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 973, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A232094: Number of nonzero digits in the factorial base representation
   of the triangular number T(n) = n*(n+1)/2 (sum of 0..n). */

#include <stddef.h>  /* for size_t if needed */

seq_t a(idx_t n) {
    /* Compute T(n) = n*(n+1)/2 using seq_t arithmetic */
    seq_t k = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Count nonzero factorial base digits while converting k */
    seq_t count = 0;
    idx_t i = 1;
    while (k != 0) {
        seq_t divisor = (seq_t)(i + 1);       /* (i+1)! division step */
        seq_t digit   = k % divisor;
        if (digit != 0) count++;
        k = k / divisor;
        i++;
    }
    return count;
}
