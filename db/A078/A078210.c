/*
 * A078210  Smallest multiple of n^2 beginning with n.
 * https://oeis.org/A078210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest multiple of n^2 whose decimal representation
 *            starts with digits of n. Computation is performed by searching
 *            increasing powers of 10
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n2 = (seq_t)n * (seq_t)n;   /* n^2 */
    seq_t pow10 = (seq_t)1;           /* 10^L, starting with L = 0 */

    for (;;)
    {
        seq_t lower = (seq_t)n * pow10;                         /* n * 10^L */
        seq_t upper = ((seq_t)(n + 1) * pow10) - (seq_t)1;     /* (n+1)*10^L - 1 */

        seq_t t = (lower + n2 - (seq_t)1) / n2;                 /* ceil(lower / n^2) */
        seq_t m = t * n2;                                      /* candidate multiple */

        if (m <= upper)
            return m;

        pow10 *= (seq_t)10;   /* next L */
    }
}
