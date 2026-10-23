/*
 * A025899  Expansion of 1/((1-x^6)*(1-x^7)*(1-x^10)).
 * https://oeis.org/A025899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2+23*n+403)/840 + (n+12)*(-1)^n/120 +
 *            ((6*n^2+5*n+3) mod 7)/7)
 * Range:     u8 n <= 449, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 64‑bit signed arithmetic – sufficient for n up to 10 000
       (and usually all values that fit in seq_t)                           */
    int64_t nn = (int64_t)n;

    /* A = n^2 + 23*n + 403                                */
    int64_t A = nn * nn + 23 * nn + 403;

    /* term2 = 7*(n+12)*(-1)^n                               */
    int64_t term2 = 7 * (nn + 12);
    if (n & 1)   /* n odd → multiply by –1 */
        term2 = -term2;

    /* r = (6*n^2 + 5*n + 3) mod 7                          */
    int64_t r = (6 * nn * nn + 5 * nn + 3) % 7;
    int64_t term3 = 120 * r;          /* 120*r corresponds to r/7 with denom 840 */

    /* Numerator of the sum with common denominator 840      */
    int64_t N = A + term2 + term3;

    /* Floor division of a potentially negative integer N by 840 */
    int64_t res = N / 840;
    if (N % 840 != 0 && N < 0)
        --res;                        /* adjust for truncation toward zero */

    return (seq_t)res;
}
