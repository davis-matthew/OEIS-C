/*
 * A221951  Number of subgroups of C_4 X C_n.
 * https://oeis.org/A221951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (1 + gcd(2,d) + gcd(4,d))
 * Range:     u8 n <= 2519, u16 n <= 9690, u32 n <= 9690, u64 n <= 9690,
 *            u128 n <= 9690, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            seq_t dd = (seq_t)d;
            sum += 1 + gcd((seq_t)2, dd) + gcd((seq_t)4, dd);

            idx_t d2 = n / d;
            if (d2 != d)          /* avoid double counting square root */
            {
                seq_t dd2 = (seq_t)d2;
                sum += 1 + gcd((seq_t)2, dd2) + gcd((seq_t)4, dd2);
            }
        }
    }
    return sum;
}
