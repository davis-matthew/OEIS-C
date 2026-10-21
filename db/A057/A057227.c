/*
 * A057227  Smallest member of smallest set S(n) of positive integers
 *          containing n which satisfies "k is in S, iff 2k-1 is in S, iff 4k
 *          is in S".
 * https://oeis.org/A057227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest member of the equivalence class of n under
 *            the rules k is in S ⇔ 2k-1 is in S ⇔ 4k is in S. The minimal
 *            element is obtained by repeatedly applying the reductions - if a
 *            number is divisible by 4, replace it by a quarter; - if it is
 *            odd, replace it by (k+1)/2. This process alwa...
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index n to the sequence type seq_t. */
    seq_t k = (seq_t)n;

    /* Stepwise reduce k until no further reduction is possible. */
    while (1)
    {
        /* The smallest possible element is 1; stop if reached. */
        if (k == (seq_t)1)
            break;

        /* If k is divisible by 4, replace it by k/4 (a strictly smaller integer). */
        if (k % (seq_t)4 == (seq_t)0)
        {
            k /= (seq_t)4;
        }
        /* If k is odd, replace it by (k+1)/2 (strictly smaller). */
        else if (k % (seq_t)2 == (seq_t)1)
        {
            k = (k + (seq_t)1) / (seq_t)2;
        }
        /* If k is even but not divisible by 4, no reduction is possible. */
        else
        {
            break;
        }
    }

    return k;
}
