/*
 * A090391  a(n) = n*(n^4 + 30*n^3 + 395*n^2 + 2910*n + 11064)/120.
 * https://oeis.org/A090391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^4 + 30*n^3 + 395*n^2 + 2910*n + 11064)/120
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 213, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t Sn = (seq_t)n;
    /* compute powers of n */
    seq_t n2 = Sn * Sn;          /* n^2 */
    seq_t n3 = n2 * Sn;          /* n^3 */
    seq_t n4 = n3 * Sn;          /* n^4 */

    /* polynomial part */
    seq_t poly = n4
               + 30 * n3
               + 395 * n2
               + 2910 * Sn
               + 11064;

    /* divide the polynomial by 120, splitting into quotient and remainder */
    seq_t q = poly / 120;
    seq_t r = poly % 120;

    /* final result: n * poly / 120 = n*(q) + (n*r)/120 */
    seq_t res = Sn * q + (Sn * r) / 120;
    return res;
}
