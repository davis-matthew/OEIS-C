/*
 * A086689  a(n) = Sum_{i=1..n} i^2*t(i), where t = A000217.
 * https://oeis.org/A086689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(12*n^2+9*n-1)/120 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 67, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use seq_t for all intermediate computations to preserve
       exactness on any integer type (up to 128‑bit) used by the build. */
    seq_t nn = (seq_t)n;
    seq_t n1 = nn + 1;
    seq_t n2 = nn + 2;

    /* 12*n^2 + 9*n - 1 */
    seq_t poly = 12 * nn * nn + 9 * nn - 1;

    /* n*(n+1)*(n+2)*(poly) */
    seq_t prod = nn * n1;
    prod *= n2;
    prod *= poly;

    /* Divide by 120 */
    return prod / 120;
}
