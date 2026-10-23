/*
 * A040542  Continued fraction for sqrt(566).
 * https://oeis.org/A040542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) is the n‑th term of the simple continued fraction of √566.
 *            The sequence begins with a(0)=⌊√566⌋=23 and then repeats the
 *            18‑term period 1,3,1,3,1,1,8,1,22,1,8,1,1,3,1,3,1,46. The
 *            implementation precomputes this period once and indexes into it
 *            for all n>0
 * Range:     u8 none, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t period[18];
    static int initialized = 0;

    if (!initialized)
    {
        const seq_t a0 = (seq_t)23;          /* floor(√566)          */
        const seq_t D  = (seq_t)566;         /* radicand             */
        seq_t m = (seq_t)0;
        seq_t d = (seq_t)1;
        seq_t a = a0;

        for (size_t i = 0; i < 18; ++i)
        {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[i] = a;
        }
        initialized = 1;
    }

    if (n == (idx_t)0)
        return (seq_t)23;

    /* n>0: return the (n‑1)-th element of the period, 0‑based */
    size_t idx = (size_t)(n - (idx_t)1) % 18;
    return period[idx];
}
