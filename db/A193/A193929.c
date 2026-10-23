/*
 * A193929  Number of prime factors of n^4 + 1, counted with multiplicity.
 * https://oeis.org/A193929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = bigomega(n^4 + 1) – the number of prime factors of n^4 +
 *            1, counted with multiplicity
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 2562, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Compute n^4 + 1 safely in seq_t */
    seq_t nn  = (seq_t)n;          /* n          */
    seq_t nn2 = nn * nn;           /* n^2        */
    seq_t nn4 = nn2 * nn2;         /* n^4        */
    seq_t m   = nn4 + 1;           /* n^4 + 1    */

    /* The result fits in idx_t for the intended range
       (n up to about 1e4).  Cast for bigomega. */
    idx_t m64 = (idx_t)m;
    return bigomega(m64);
}
