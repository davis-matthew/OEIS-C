/*
 * A239704  Number of bases b for which the base-b alternate digital sum of n
 *          is b.
 * https://oeis.org/A239704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns a(n), the number of bases b (including base 1) for which
 * the base‑b alternate digit sum of n equals b.
 * The alternation follows the convention of OEIS A055017:
 *   least significant digit is added, next digit subtracted, and so on.
 *
 * The calculation is done by direct simulation, which is fast enough for
 * n up to about 10 000 (≈140 000 digit operations).
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Count the base‑1 case: altDigitSum_1(n) = 1 if n odd, 0 if n even. */
    seq_t count = (n & 1) ? 1 : 0;

    /* Skip the trivial n==0 case; for n>=1 it does nothing. */
    if (n < 2)
        return count;

    /* Test all bases 2 … n.  For b>n the representation is a single digit
     * and altDigitSum_b(n)=+n, so the only base that can satisfy the
     * condition is b==n, which is already included in the loop. */
    for (idx_t b = 2; b <= n; ++b)
    {
        idx_t temp = n;
        long long sum = 0;   /* signed sum of alternate digits */
        int sign = 1;        /* +1 for least significant digit, then -1, +1, … */

        while (temp > 0)
        {
            idx_t digit = temp % b;
            sum += sign * (long long)digit;
            sign = -sign;
            temp /= b;
        }

        if (sum == (long long)b)
            ++count;
    }

    return count;
}
