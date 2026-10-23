/*
 * A136105  Partial sums of A051941.
 * https://oeis.org/A136105
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n-6)(n-7)(n^2+19n-144)/24, n >= 8
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;                    /* undefined for n < 8, values are irrelevant */

    seq_t N  = (seq_t)n;
    seq_t t1 = N - (seq_t)6;          /* n - 6 */
    seq_t t2 = N - (seq_t)7;          /* n - 7 */
    seq_t t3 = N * N + (seq_t)19 * N - (seq_t)144; /* n^2 + 19n - 144 */

    seq_t prod = t1 * t2;             /* (n-6)(n-7) */
    prod = prod * t3;                 /* multiply by (n^2+19n-144) */

    return prod / (seq_t)24;          /* divide by 24, exact integer division */
}
