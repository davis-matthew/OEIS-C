/*
 * A174018  Number of reduced 3 X 3 magilatin squares with largest entry n.
 * https://oeis.org/A174018
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(2*n^3-21*n^2+106*n-172) + (n mod 2)*(9*n^2-108*n+306)
 *            + 24*(n-6)*((2*n^2+1) mod 3) + 48*(n mod 3) - 144*( [n mod 4 =
 *            0] + [n mod 5 = 0] )
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 898, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                       /* behaviour below the offset is irrelevant */

    /* Use signed 64‑bit types for intermediate arithmetic.
       n is known to be small enough that 64‑bit signed arithmetic
       suffices for n up to at least 10⁶ (where a(n) < 6·10¹⁸). */
    int64_t N  = (int64_t)n;              /* safe conversion: n ≥ 2 */
    int64_t n2 = N * N;
    int64_t n3 = n2 * N;

    int64_t t1 = 3 * (2 * n3 - 21 * n2 + 106 * N - 172);

    int64_t t2 = 0;
    if (N & 1)                            /* n mod 2 == 1 */
        t2 = 9 * n2 - 108 * N + 306;

    int64_t t3 = 0;
    if (N % 3 == 0)                       /* (2*n^2+1) mod 3 == 1 only when n%3==0 */
        t3 = 24 * (N - 6);

    int64_t t4 = 48 * (N % 3);

    int64_t t5 = 0;
    if (N % 4 == 0)
        t5 += 144;
    if (N % 5 == 0)
        t5 += 144;

    int64_t result = t1 + t2 + t3 + t4 - t5;

    return (seq_t)result;
}
