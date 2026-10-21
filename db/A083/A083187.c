/*
 * A083187  a(n) = floor(log_3/2(n+1)) - floor(log_3/2(n)).
 * https://oeis.org/A083187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(log_{3/2}(n+1)) - floor(log_{3/2}(n))
 *            (equivalently, a(n)=1 iff n = ⌊(3/2)^k⌋ for some k≥0)
 * Range:     u8 n <= 9452, u16 n <= 9452, u32 n <= 9452, u64 n <= 9452,
 *            u128 n <= 9452, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 by definition, but handle n==0 safely */
    if (n == 0)
        return (seq_t)0;

    /* Iterate over powers of 3 and 2 to find the floor value. */
    idx_t pow3 = 1;          /* 3^k  */
    idx_t pow2 = 1;          /* 2^k  */

    for (idx_t k = 0; ; ++k)
    {
        idx_t val = pow3 / pow2;    /* ⌊ (3^k) / (2^k) ⌋  */

        if (val == n)                /* n is a threshold value */
            return (seq_t)1;

        if (val > n)                 /* further values will be larger */
            break;

        /* Prepare for next k.  The loop is very short for the
           constraints of the test suite (n up to a few thousand). */
        /* Detect overflow before multiplying.  If overflow would
           occur, the loop would certainly terminate because `val`
           would exceed any possible n. */
        if (pow3 > (UINT64_MAX / 3) || pow2 > (UINT64_MAX / 2))
            break;

        pow3 *= 3;
        pow2 *= 2;
    }

    return (seq_t)0;
}
