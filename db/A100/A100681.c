/*
 * A100681  Inverse modulo 2 binomial transform of 10^n.
 * https://oeis.org/A100681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..n} (-1)^A010060(n-k) * mod(binomial(n,k),2) *
 *            10^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t pos = 0;
    seq_t neg = 0;
    idx_t k = n;
    for (;;)
    {
        /* compute popcount of n-k to decide the sign */
        idx_t diff = n - k;
        int pc = __builtin_popcountll((unsigned long long)diff);
        seq_t term = pow10(k);
        if ((pc & 1) == 0)
            pos += term;      /* even popcount -> + */
        else
            neg += term;      /* odd popcount  -> - */

        if (k == 0)
            break;
        k = (k - 1) & n;      /* next submask of n */
    }
    /* pos >= neg for this sequence, so safe to subtract */
    seq_t res = pos - neg;
    return res;
}
