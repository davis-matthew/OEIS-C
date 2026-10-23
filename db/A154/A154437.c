/*
 * A154437  Permutation of nonnegative integers: A059893-conjugate of A154435.
 * https://oeis.org/A154437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A054429(A284459(n)) = A258996(A153154(n)) =
 *            A284459(A065190(n)). The recurrence for n > 0 is: a(1) = 1
 *            a(2*n) = 2 * a( A065190(n) ) + 1 a(2*n+1) = 2 * a( n )
 * Range:     u8 n <= 255, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   A065190
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A065190.h"

/* Main function for sequence A154437 */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    if ((n & 1) == 0)  /* even */
    {
        idx_t m = n >> 1;            /* n = 2*m */
        return ((seq_t)2 * a(A065190(m))) + 1;
    }
    else                /* odd: n = 2*m + 1 */
    {
        idx_t m = (n - 1) >> 1;      /* n = 2*m + 1 */
        return ((seq_t)2 * a(m));
    }
}
