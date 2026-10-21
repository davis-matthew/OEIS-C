/*
 * A219396  Numbers k such that 19*k+1 is a square.
 * https://oeis.org/A219396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (38*n*(n-1) + 15*(-1)^n*(2*n - 1)-1)/8 + 2
 * Range:     u8 n <= 7, u16 n <= 117, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 64‑bit signed arithmetic to avoid overflow and sign errors. */
    int64_t n64 = (int64_t)n;
    int64_t num = 38LL * n64 * (n64 - 1);

    /* Add or subtract the (-1)^n term. */
    if ((n64 & 1) == 0)          /* n even  => (-1)^n =  1 */
        num += 15LL * (2 * n64 - 1);
    else                         /* n odd   => (-1)^n = -1 */
        num -= 15LL * (2 * n64 - 1);

    num -= 1;                    /* subtract the final 1 */
    int64_t res = num / 8 + 2;   /* division is exact; add 2 */

    return (seq_t)res;
}
