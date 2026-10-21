/*
 * A084096  Fifth row of number array A084061.
 * https://oeis.org/A084096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n-2)^n + (n+2)^n) / 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   pow_seq_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                       /* for size_t, if needed */
#include "../utils/pow_seq_local.h"       /* pow_seq_local(seq_t base, int exp) */

seq_t a(idx_t n)
{
    /* Compute (n+2)^n */
    seq_t term1 = pow_seq_local((seq_t)(n + 2), (int)n);

    /* Compute |n-2|^n, then add or subtract depending on sign and parity */
    seq_t term2;
    if (n >= 2)
    {
        /* (n-2) is non‑negative */
        term2 = pow_seq_local((seq_t)(n - 2), (int)n);
    }
    else
    {
        /* (n-2) is negative; use absolute value for the power */
        idx_t absdiff = 2 - n;           /* 2 - n = |n-2|  when n<2 */
        term2 = pow_seq_local((seq_t)absdiff, (int)n);
    }

    seq_t sum;
    if (n < 2 && (n & 1u))
    {
        /* (n-2)^n is negative and n is odd: subtract */
        sum = term1 - term2;
    }
    else
    {
        /* Regular addition */
        sum = term1 + term2;
    }

    /* Division by 2 is exact for all n */
    return sum / 2;
}
