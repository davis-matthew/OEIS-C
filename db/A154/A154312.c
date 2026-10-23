/*
 * A154312  Triangle T(n,k), 0<=k<=n, read by rows, given by
 *          [0,1/2,-1/2,0,0,0,0,0,0,0,...] DELTA [2,-1/2,-1/2,2,0,0,0,0,0,0,0
 *          ...] where DELTA is the operator defined in A084938 .
 * https://oeis.org/A154312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(0) = 1, a(1) = 0, a(2) = 2, a(3) = 0. For n ≥ 4: let t ≥ 1
 *            such that k = (t² + 5t + 2)/2. If n = k → a(n) = 2ʳ – 1, if n =
 *            k+1 → a(n) = 2ʳ + 1, else a(n) = 0. (t is the unique integer for
 *            which k or k+1 equals n.) This reproduces OEIS A154312
 * Range:     u8 n <= 53, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* handle the first few special cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return pow2((idx_t)1);   /* = 2 */
    if (n == 3) return (seq_t)0;

    /* search for the matching t */
    for (idx_t t = 1; ; ++t)
    {
        idx_t k = (t * t + 5 * t + 2) / 2;   /* start of the pair for this t */

        if (k == n)          /* first of the pair */
        {
            seq_t v = pow2(t);
            return v - (seq_t)1;
        }
        if (k + 1 == n)      /* second of the pair */
        {
            seq_t v = pow2(t);
            return v + (seq_t)1;
        }
        if (k > n) break;    /* n is beyond all remaining pairs */
    }

    return (seq_t)0;          /* all other indices give 0 */
}
