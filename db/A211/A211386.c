/*
 * A211386  Expansion of 1/((1-2*x)^5*(1-x)).
 * https://oeis.org/A211386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(24 + 18*n + 23*n^2 + 6*n^3 + n^4)/12 - 1
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only integer arithmetic on the supplied types seq_t and idx_t */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Compute 2^n using left shift on seq_t */
    seq_t power2 = ((seq_t)1) << n;

    /* Convert n to seq_t for further calculations */
    seq_t nn   = (seq_t)n;
    seq_t nn2  = nn * nn;       /* n^2 */
    seq_t nn3  = nn2 * nn;      /* n^3 */
    seq_t nn4  = nn3 * nn;      /* n^4 */

    /* Inner polynomial: 24 + 18*n + 23*n^2 + 6*n^3 + n^4 */
    seq_t inner = (seq_t)24
                + (seq_t)18 * nn
                + (seq_t)23 * nn2
                + (seq_t)6  * nn3
                + nn4;

    /* Final result: 2^n * inner / 12 - 1 */
    seq_t result = (power2 * inner) / (seq_t)12 - (seq_t)1;
    return result;
}
