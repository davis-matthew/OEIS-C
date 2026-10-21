/*
 * A224457  The hyper-Wiener index of the cyclic phenylene with n hexagons
 *          (n>=3).
 * https://oeis.org/A224457
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3/2) n (2 n^3 + 15 n^2 + 45 n - 88)
 * Range:     u8 none, u16 n <= 10, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /*  For n < 3 the definition is not required; return 0. */
    if (n < 3) return (seq_t)0;

    seq_t N    = (seq_t)n;
    seq_t term = (seq_t)2 * N * N * N          /* 2 n^3   */
              + (seq_t)15 * N * N              /* 15 n^2  */
              + (seq_t)45 * N                  /* 45 n    */
              - (seq_t)88;                    /* -88     */

    seq_t prod = N * term;                     /* n * term */
    seq_t res  = (prod * (seq_t)3) / (seq_t)2; /* (3/2) * prod */

    return res;
}
