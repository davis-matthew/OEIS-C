/*
 * A105519  Number of times 9 is the leading digit of the first n+1 Fibonacci
 *          numbers in decimal representation.
 * https://oeis.org/A105519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{k ≤ n : leading digit of Fibonacci F(k) = 9}. For k ≥
 *            17 the fractional‑log method is used
 * Range:     u8 n <= 5599, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <math.h>

/* Leading digits of F(0) … F(16) */
static const unsigned char leading_small[17] = {
    0, 1, 1, 2, 3, 5, 8, 1, 2, 3, 5, 8, 1, 2, 3, 6, 9
};

#define LOG10_PHI   0.20898764024997873
#define LOG10_SQRT5 0.34948500216800945
#define EPS 1e-12

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t i;

    for (i = 0; i <= n; ++i) {
        unsigned digit;
        if (i < 17) {
            digit = leading_small[i];
        } else {
            double x = (double)i * LOG10_PHI - LOG10_SQRT5;
            double intpart;
            double f = modf(x, &intpart);   /* fractional part of x */
            double v = pow(10.0, f);
            digit = (unsigned)(v + EPS);    /* integer part of 10^f */
        }
        if (digit == 9)
            ++count;
    }

    return count;
}
