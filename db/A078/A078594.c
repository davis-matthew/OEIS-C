/*
 * A078594  Number of pairs (x,y) 1<=x<=y<=n such that 1/x+1/y+1/n > 1/2.
 * https://oeis.org/A078594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (x,y) with 1 <= x <= y <= n and
 *            1/x+1/y+1/n > 1/2. For n >= 43 the sequence equals 2*n + 4;
 *            otherwise it is computed by direct O(n) counting
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* fast formula for large n */
    if (n >= 43)
        return (seq_t)(2 * (int64_t)n + 4);

    int64_t N = (int64_t)n;
    if (N <= 0)
        return 0;

    /* S = (n-2)/(2n) represented by numerator/denominator */
    int64_t Snum = N - 2;           /* may be negative for n = 1 */
    int64_t Sden = 2 * N;           /* > 0 */

    int64_t count = 0;
    for (int64_t x = 1; x <= N; ++x) {
        if (Snum * x <= Sden) {
            /* inequality holds for all y in [x, n] */
            count += N - x + 1;
        } else {
            /* need y with 1/y > S - 1/x  =>  y < 1/D  where D = S - 1/x */
            int64_t dnum = Snum * x - Sden;   /* > 0 */
            int64_t dden = Sden * x;
            int64_t ymax = (dden - 1) / dnum;
            if (ymax > N)
                ymax = N;
            if (ymax >= x)
                count += ymax - x + 1;
        }
    }
    return (seq_t)count;
}
