/*
 * A101090  Third partial sums of fourth powers (A000583).
 * https://oeis.org/A101090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(3+2*n)*(-1+2*n*(n+3))/840
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 49, u64 n <= 1211,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* formula is defined for n >= 1; for n == 0 return 0 (unused
       by the problem).  The calculation is performed in an
       unsigned __int128 intermediate to avoid overflow.  The final
       value is cast to seq_t. */
    if (n == 0)
        return 0;

    unsigned __int128 N = n;
    unsigned __int128 prod = N;                /* n */
    prod *= (N + 1);                          /* n*(n+1) */
    prod *= (N + 2);                          /* n*(n+1)*(n+2) */
    prod *= (N + 3);                          /* n*(n+1)*(n+2)*(n+3) */
    prod *= (3 + 2 * N);                      /* * (3+2n) */
    prod *= (2 * N * (N + 3) - 1);            /* * (-1+2n(n+3)) */
    prod /= 840;                              /* divide by 840 */

    return (seq_t)prod;
}
