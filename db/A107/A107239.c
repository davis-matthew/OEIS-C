/*
 * A107239  Sum of squares of tribonacci numbers (A000073).
 * https://oeis.org/A107239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{i=0}^n T(i)^2 where T(0)=0, T(1)=0, T(2)=1 and
 *            T(k)=T(k-1)+T(k-2)+T(k-3) for k≥3
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Tribonacci seeds */
    seq_t t0 = 0;  /* T(k-2) */
    seq_t t1 = 0;  /* T(k-1) */
    seq_t t2 = 1;  /* T(k)   */
    seq_t sum = 0; /* Result a(n) */

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t t;
        if (i == 0)
            t = t0;
        else if (i == 1)
            t = t1;
        else if (i == 2)
            t = t2;
        else
            t = t0 + t1 + t2; /* T(k) for k>=3 */

        sum += t * t;

        if (i >= 3) {
            t0 = t1;  /* shift window for next iteration */
            t1 = t2;
            t2 = t;
        }
    }
    return sum;
}
