/*
 * A143369  The Szeged index of fourth type dendrimer nanostar which has grown
 *          n stages.
 * https://oeis.org/A143369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24624*n*4^n + 7156 + 25992*n*2^n + 53532*2^n - 57024*4^n
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t pow4n = pow4(n);          /* 4^n */
    seq_t pow2n = pow2(n);          /* 2^n */

    seq_t term1 = pow4n * (((seq_t)24624 * nn) - ((seq_t)57024));
    seq_t term2 = pow2n * (((seq_t)25992 * nn) + ((seq_t)53532));

    return term1 + term2 + ((seq_t)7156);
}
