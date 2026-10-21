/*
 * A209723  1/4 the number of (n+1) X 5 0..2 arrays with every 2 X 2 subblock
 *          having distinct clockwise edge differences.
 * https://oeis.org/A209723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^((n+1)/2)+4 for n odd, a(n) = 3*2^(n/2-1)+4 for n even
 *            (n≥1)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t pow2seq(idx_t e)
{
    seq_t r = (seq_t)1;
    while (e--)
        r += r;       /* r *= 2 */
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset not defined; safe default */
        return (seq_t)0;
    if (n & (idx_t)1)           /* odd index */
    {
        idx_t exp = (n + (idx_t)1) / 2;
        return pow2seq(exp) + (seq_t)4;
    }
    else                        /* even index */
    {
        idx_t exp = n / 2 - (idx_t)1;
        return (seq_t)3 * pow2seq(exp) + (seq_t)4;
    }
}
