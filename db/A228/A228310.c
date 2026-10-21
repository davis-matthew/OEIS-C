/*
 * A228310  The hyper-Wiener index of the hypercube graph Q(n) (n>=2).
 * https://oeis.org/A228310
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^{n-2} * n * (n+3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n < 2)
    {
        return 0;            /* undefined for n < 2, return 0 */
    }

    seq_t power = pow4(n - 2);    /* 4^(n-2) */
    seq_t sn     = (seq_t)n;      /* cast index to seq_t */
    seq_t term   = sn * (sn + (seq_t)3); /* n*(n+3) */
    return power * term;
}
