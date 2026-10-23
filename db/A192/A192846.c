/*
 * A192846  Molecular topological indices of the sunlet graphs.
 * https://oeis.org/A192846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n*(n+3)+(-1)^n+7)
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;                     /* cast index to seq_t */
    seq_t term = (seq_t)2 * N * (N + (seq_t)3);  /* 2*n*(n+3) */
    term += (seq_t)7;                      /* +7 */
    if (n & 1)                              /* odd n: subtract 1 */
        term -= (seq_t)1;
    else                                    /* even n: add 1 */
        term += (seq_t)1;
    return N * term;                        /* n * term */
}
