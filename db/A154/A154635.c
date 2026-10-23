/*
 * A154635  Ratio of the sum of the bends of the 5-dimensional spheres added
 *          in the n-th generation of Apollonian packing to the sum of the
 *          bends of the initial configuration of seven mutually tangent
 *          spheres.
 * https://oeis.org/A154635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=15; for n>=3: a(n)=8*a(n-1)-6*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses only integer arithmetic and is
   independent of any external allocation functions.  It works
   for any order of calls and for n up to 10 000. */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }
    if (n == (idx_t)1) {
        return (seq_t)2;
    }
    if (n == (idx_t)2) {
        return (seq_t)15;
    }

    /* iterate from 3 up to n, keeping only the last two values */
    seq_t prev2 = (seq_t)2;    /* a(n-2) for n=3 is a(1) */
    seq_t prev1 = (seq_t)15;   /* a(n-1) for n=3 is a(2) */
    seq_t curr  = (seq_t)0;    /* placeholder for a(n) */

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        curr = ((seq_t)8) * prev1 - ((seq_t)6) * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;   /* curr now holds a(n) */
}
