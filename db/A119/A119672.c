/*
 * A119672  Number of prime factors (with multiplicity) of n^4 + 3*n^2 + 1
 *          (A057721).
 * https://oeis.org/A119672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Bigomega(n^4 + 3*n^2 + 1)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Compute n^4 + 3*n^2 + 1 */
    idx_t n2 = n * n;          /* n^2  */
    idx_t n4 = n2 * n2;        /* n^4  */
    idx_t val = n4 + 3 * n2 + 1; /* n^4 + 3*n^2 + 1 */

    /* Return the number of prime factors (with multiplicity) */
    return bigomega(val);
}
