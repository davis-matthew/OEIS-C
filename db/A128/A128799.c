/*
 * A128799  a(n) = n*(n-1)*5^n.
 * https://oeis.org/A128799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*5^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 23, u128 n <= 50,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    if (n <= 1u)
        return 0;

    seq_t nn      = (seq_t)n;
    seq_t nminus1 = (seq_t)(n - 1u);
    seq_t pow5    = power5(n);        /* 5^n */

    return nn * nminus1 * pow5;
}
