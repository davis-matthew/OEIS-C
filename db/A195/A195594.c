/*
 * A195594  Number of ways to place 6n nonattacking kings on a vertical
 *          cylinder 12 X 2n.
 * https://oeis.org/A195594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15839*n - 99729)*2^n + (231*n^4)/2 + 1767*n^3 +
 *            (26001*n^2)/2 + 53295*n + 99730 * Recurrence: a(n) = 4*a(n-7) -
 *            24*a(n-6) + 61*a(n-5) - 85*a(n-4) + 70*a(n-3) - 34*a(n-2) +
 *            9*a(n-1); * Indexing: a(1) = 128, a(2) = 2102, … (offset = 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 14, u64 n <= 44, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0, not needed */

    /* 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) pow2 = pow2 * (seq_t)2;

    seq_t sn = (seq_t)n;

    /* (15839*n)*2^n */
    seq_t term1 = pow2;
    term1 = term1 * (seq_t)15839;
    term1 = term1 * sn;

    /* 99729*2^n */
    seq_t term2 = pow2 * (seq_t)99729;

    /* n^2, n^3, n^4 */
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;
    seq_t n4 = n2 * n2;

    /* polynomial part: (231*n^4 + 26001*n^2)/2 + 1767*n^3 + 53295*n + 99730 */
    seq_t part = (seq_t)231 * n4 + (seq_t)26001 * n2;
    part = part / (seq_t)2;
    seq_t poly = part + (seq_t)1767 * n3 + (seq_t)53295 * sn + (seq_t)99730;

    /* a(n) = (15839*n)*2^n + poly - 99729*2^n */
    seq_t result = term1 + poly;
    result -= term2;

    return result;
}
