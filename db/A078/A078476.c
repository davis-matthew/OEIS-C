/*
 * A078476  Time taken to get n people from one side of a bridge to the other
 *          where (a) the only flashlight must be carried when crossing; (b)
 *          only one or two people may cross at the same time; (c) a pair
 *          crosses at the speed of the slowest member; and (d) the k-th
 *          person's speed requires k seconds to cross...
 * https://oeis.org/A078476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1 For n > 1: if n even: a(n) = (n^2 + 12*n - 20)/4 if n
 *            odd : a(n) = (n^2 + 12*n - 21)/4
 * Range:     u8 n <= 11, u16 n <= 505, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;                       /* a(1) = 1 */

    /* cast index to sequence type before arithmetic */
    seq_t nn  = (seq_t)n;
    seq_t tmp = nn * nn + (seq_t)12 * nn;     /* n^2 + 12*n */

    /* subtract 20 for even n, 21 for odd n */
    if ((n & 1ULL) == 0)
        tmp -= (seq_t)20;          /* even n */
    else
        tmp -= (seq_t)21;          /* odd n */

    return tmp / (seq_t)4;          /* integer division */
}
