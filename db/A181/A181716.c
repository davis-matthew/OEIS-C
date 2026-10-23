/*
 * A181716  a(n) = a(n-1) + a(n-2) + (-1)^n, with a(0)=0 and a(1)=1.
 * https://oeis.org/A181716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + (-1)^n, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)1;   /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t sum = prev1 + prev2;
        if ((i & 1) == 0)       /* even i: add +1 */
            cur = sum + (seq_t)1;
        else                     /* odd i: subtract -1 */
            cur = sum - (seq_t)1;

        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
