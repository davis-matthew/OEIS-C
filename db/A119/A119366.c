/*
 * A119366  Number of rooted planar n-trees whose number of leaves is equal to
 *          1 modulo 3.
 * https://oeis.org/A119366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0}^{⌊n/3⌋} (C(n,3k+1)*C(n,3k))/n with a(0)=0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

/* Return the n-th term of sequence A119366 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* a(0) = 0 */

    seq_t sum = 0;
    idx_t max_k = n / 3;             /* floor(n/3) */

    for (idx_t k = 0; k <= max_k; ++k)
    {
        idx_t j   = 3 * k;           /* 3k */
        idx_t jp1 = j + 1;           /* 3k+1 */

        seq_t c_j   = binom(n, j);
        seq_t c_jp1 = binom(n, jp1);

        /* product divided by n, exact division */
        seq_t prod = c_j * c_jp1;
        seq_t term = prod / (seq_t)n;
        sum += term;
    }

    return sum;
}
