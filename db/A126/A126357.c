/*
 * A126357  a(n) is the (n+1)st integer from among the positive integers which
 *          are coprime to n.
 * https://oeis.org/A126357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the (n+1)th positive integer that is coprime to n
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* target is the order of the desired coprime, i.e. n+1 */
    seq_t target = (seq_t)n + 1;
    seq_t count  = 0;
    seq_t x      = 1;                /* current candidate integer */

    /* Iterate over positive integers until we find the target-th coprime. */
    while (1)
    {
        if (gcd((seq_t)n, x) == 1)
        {
            ++count;
            if (count == target)
                return x;
        }
        ++x;
    }
}
