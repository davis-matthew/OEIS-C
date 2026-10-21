/*
 * A091916  Maximum of odd products of partitions of n.
 * https://oeis.org/A091916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=a(2)=1, a(3n)=3^n, a(3n+1)=a(3n), a(3n+2)=5·3^(n-1)
 *            (for n≥1)
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow3(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= (seq_t)3;
    return r;
}

seq_t a(idx_t n)
{
    if (n <= 2)
        return 1;                    /* a(0)=a(1)=a(2)=1 */

    const idx_t r = n % 3;
    if (r == 0)                      /* n = 3k */
    {
        idx_t k = n / 3;
        return pow3(k);              /* a(3k)=3^k */
    }
    else if (r == 1)                 /* n = 3k+1 */
    {
        idx_t k = n / 3;
        return pow3(k);              /* a(3k+1)=a(3k)=3^k */
    }
    else                             /* r == 2, n = 3k+2 with k≥1 */
    {
        idx_t m = (n - 2) / 3;       /* m = k, m≥1 */
        seq_t r = 1;
        for (idx_t i = 0; i < m - 1; ++i)  /* exponent m-1 */
            r *= (seq_t)3;
        r *= (seq_t)5;                /* a(3m+2)=5·3^(m-1) */
        return r;
    }
}
