/*
 * A101613  (2n)! * Sum[k=n..2n, 1/k].
 * https://oeis.org/A101613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n)! * Σ_{k=n}^{2n} (1/k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute (2n)! */
    idx_t twon = n + n;              /* 2 * n */
    seq_t fac = 1;
    for (idx_t i = 2; i <= twon; ++i)
        fac *= (seq_t)i;

    /* compute the sum of fac / k for k = n .. 2n */
    seq_t sum = 0;
    for (idx_t k = n; k <= twon; ++k)
        sum += fac / (seq_t)k;

    return sum;
}
