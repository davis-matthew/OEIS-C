/*
 * A205379  The index j<k such that n divides s(k)-s(j) for some j, where
 *          s(j)=(2j-1)^2.
 * https://oeis.org/A205379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest j < k such that n divides (2k-1)^2 -
 *            (2j-1)^2 (equivalently n | 4*(k-j)*(k+j-1))
 * Range:     u8 n <= 36, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t g = gcd_u64(n, (idx_t)4);
    idx_t m = n / g;                /* m = n / gcd(n,4)  */
    seq_t mseq = (seq_t)m;          /* cast to seq_t for modulo */

    for (idx_t k = 2; ; ++k)        /* search increasing k */
    {
        for (idx_t d = k - 1; d; --d)   /* d = k - j  (descending) */
        {
            seq_t prod = (seq_t)d * (seq_t)(2 * k - d - 1);
            if (prod % mseq == 0)      /* m divides d*(2k-d-1) */
                return (seq_t)(k - d);  /* j = k - d */
        }
    }
}
