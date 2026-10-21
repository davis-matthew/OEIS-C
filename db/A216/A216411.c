/*
 * A216411  Number of bases in which n begins with a "1".
 * https://oeis.org/A216411
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of bases b ≥ 2 in which the representation of n
 *            starts with the digit 1. It is counted by checking the
 *            inequality n < 2 * b^⌊log_b n⌋
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation iterates over all bases 2..n and uses only
    integer arithmetic, avoiding overflow by performing comparisons
    before multiplication. */

#include <stdint.h>   /* for standard integer types, if needed */

seq_t a(idx_t n)
{
    seq_t count = (seq_t)0;
    seq_t nn = (seq_t)n;          /* cast index to arithmetic type */

    for (idx_t base = (idx_t)2; base <= n; ++base)
    {
        seq_t bb = (seq_t)base;   /* convert loop variable to seq_t */
        seq_t lower = bb;         /* lowest power b^p such that b^p ≤ n */

        /* find largest power <= n without overflow: multiply only when
           lower * b will not exceed nn. */
        while (lower <= nn / bb)
        {
            lower *= bb;
        }

        /* check n < 2 * lower  =>  lower > n/2  */
        if (lower > nn / (seq_t)2)
        {
            ++count;
        }
    }
    return count;
}
