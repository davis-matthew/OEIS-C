/*
 * A180290  Number of arrangements of n indistinguishable balls in n boxes
 *          with the maximum number of balls in any box equal to 10.
 * https://oeis.org/A180290
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = g(n,10) - g(n,9) where g(n,B) = ∑_{k=0}^{⌊n/(B+1)⌋}
 *            (-1)^k · C(n,k) · C(2n-1-k(B+1), n-1)
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 21, u64 n <= 37, u128 n <= 68,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count weak compositions of n into n parts with each part ≤ B. */
#include "../utils/binom.h"

static seq_t g(idx_t n, idx_t B)
{
    if (n == 0)                    /* empty composition */
        return 1;
    idx_t maxk = n / (B + 1);
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t c1 = binom(n, k);
        idx_t rem = 2 * n - 1 - k * (B + 1);
        seq_t c2 = binom(rem, n - 1);
        seq_t prod = c1 * c2;
        if (k & 1)
            sum -= prod;
        else
            sum += prod;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 10)
        return 0;                        /* sequence defined for n ≥ 10 */
    seq_t up   = g(n, 10);               /* max part ≤ 10 */
    seq_t low  = g(n, 9);                /* max part ≤ 9  */
    return up - low;                    /* max part exactly 10 */
}
