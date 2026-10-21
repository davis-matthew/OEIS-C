/*
 * A052779  Expansion of e.g.f.: (log(1-x))^6.
 * https://oeis.org/A052779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 720 * S(n,6), where S(n,6) is the unsigned Stirling
 *            number of the first kind. For n < 6, a(n) = 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses an O(n) algorithm that iteratively
   computes the unsigned Stirling numbers up to order 6.
   It works for sequence values that fit in seq_t or as arbitrary
   precision when seq_t is a big-integer class. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 6)
        return (seq_t)0;

    /* Array for unsigned Stirling numbers S(k) = S(current_n, k) */
    seq_t S[7] = {0};
    S[0] = (seq_t)1;          /* S(0,0) = 1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Update from k = 6 down to 1 to avoid overwriting needed values */
        for (idx_t k = 6; k >= 1; --k)
        {
            /* S(i,k) = S(i-1,k-1) + (i-1) * S(i-1,k) */
            S[k] = S[k - 1] + ((seq_t)(i - 1)) * S[k];
        }
        S[0] = (seq_t)0;   /* S(i,0) = 0 for i > 0 */
    }

    /* Final result: 720 * S(n,6) */
    return S[6] * (seq_t)720;
}
