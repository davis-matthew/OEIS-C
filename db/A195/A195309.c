/*
 * A195309  Row sums of an irregular triangle read by rows in which row n
 *          lists the next A026741(n+1) natural numbers A000027.
 * https://oeis.org/A195309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(9*n^2+18*n-1+(3*n^2+6*n+1)*(-1)^n)/32
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* Return 0 for n<=0, though OEIS offset starts at 1 */
    if (n == 0) return 0;
    /* Work with signed 64‑bit intermediate */
    long long nn = (long long)n;
    long long t1 = 9*nn*nn + 18*nn - 1;          /* 9*n^2 + 18*n - 1 */
    long long t2 = 3*nn*nn + 6*nn + 1;           /* 3*n^2 + 6*n + 1 */
    long long sum = t1 + ((nn & 1LL) ? -t2 : t2);/* add or subtract t2 depending on n parity */
    long long res = (nn + 1) * sum / 32;         /* final division by 32 */
    return (seq_t)res;
}
