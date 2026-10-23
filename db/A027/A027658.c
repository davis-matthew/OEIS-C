/*
 * A027658  a(n) = binomial(n+2, 2) + binomial(n+4, 5).
 * https://oeis.org/A027658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+2,2) + binomial(n+4,5)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 199, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* binomial(n+2, 2) = (n+2)*(n+1)/2 */
    seq_t t1 = (nn + 2) * (nn + 1) / 2;

    /* binomial(n+4, 5) = (n+4)*(n+3)*(n+2)*(n+1)*n / 120 */
    seq_t t2 = (nn + 4) * (nn + 3) * (nn + 2) * (nn + 1) * nn / 120;

    return t1 + t2;
}
