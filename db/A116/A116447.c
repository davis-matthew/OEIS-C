/*
 * A116447  a(2n) = n, a(2n+1) = n!.
 * https://oeis.org/A116447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n) = n, a(2n+1) = n!
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 26, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t fact_u(idx_t m)
{
    seq_t r = 1;
    for (idx_t i = 2; i <= m; ++i)
        r *= (seq_t)i;
    return r;
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0)      /* even index: a(2k) = k */
        return (seq_t)(n / 2);
    else                   /* odd index: a(2k+1) = k! */
    {
        idx_t k = (n - 1) / 2;
        return fact_u(k);
    }
}
