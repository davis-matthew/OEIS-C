/*
 * A137885  Number of directed Hamiltonian paths in the 2n-crossed prism
 *          graph.
 * https://oeis.org/A137885
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12 * 2^(n-1) * n * (n+1) (offset: 2)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                          /* undefined for n < 2 */

    seq_t result = (seq_t)12;

    result *= pow2(n - 1);                  /* 2^(n-1) */
    result *= (seq_t)n;                     /* n */
    result *= ((seq_t)n) + 1;               /* (n+1) */

    return result;
}
