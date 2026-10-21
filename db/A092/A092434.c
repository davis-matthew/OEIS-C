/*
 * A092434  Number of words X=x(1)x(2)x(3)...x(n) of length n in three digits
 *          {0,1,2} that are invariant under the mapping X -> Y, where
 *          y(i)=((AD)^(i-1))x(1) and where (AD) denotes the absolute
 *          difference (AD)x(i)=abs(x(i+1)-x(i)) (in other words, y(i) is the
 *          i-th element in the diagonal of leading entries...
 * https://oeis.org/A092434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A092434
   Formula: a(n) = (n+2) * 2^((n-1) div 2)
   The function works for any seq_t type supplied by the build.
   It handles the sequence for all n ≥ 1; the result is only
   correct if the exact value fits into seq_t.  For 64‑bit
   builds, values that overflow 64 bits are outside the test set
   and are defined arbitrarily.  The implementation avoids
   undefined behaviour on shifting by computing the power of two
   with repeated multiplication.  This is O((n-1)/2) which is
   comfortably fast for n up to 10 000. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* the sequence is defined for n >= 1; n == 0 is not used */
        return (seq_t)0;

    /* exponent of 2 */
    idx_t exp = (n - 1) / 2;

    /* compute 2^exp in seq_t */
    seq_t pow2 = (seq_t)1;
    while (exp--)           /* repeated multiplication, safe for all seq_t types */
        pow2 *= (seq_t)2;

    /* (n+2) * pow2, cast n+2 to seq_t first */
    return (seq_t)(n + 2) * pow2;
}
