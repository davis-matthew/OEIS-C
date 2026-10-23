/*
 * A039936  Smallest k for which k, 2k, ... nk all contain the digit 5.
 * https://oeis.org/A039936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 5^ceil(log2(n+1))
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 8191, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the smallest positive integer k such that k, 2k, … , nk all contain the digit 5.
 * Observation: k = 5^m satisfies the requirement for n up to 2^m – 1,
 * because 2^m * 5^m = (2·5)^m = 10^m has no digit 5.
 * Therefore a(n) = 5^ceil(log2(n+1)). */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* not used for offset 1, but harmless */

    idx_t t = n + 1;              /* t > 0 */

    /* compute floor(log2(t)) */
    int fl = 0;
    idx_t tmp = t >> 1;
    while (tmp) {
        ++fl;
        tmp >>= 1;
    }

    /* ceil(log2(t)) = floor if t is power of two, else floor+1 */
    int ceil_log2 = ((t & (t - 1)) == 0) ? fl : fl + 1;

    /* compute 5^ceil_log2 */
    seq_t res = 1;
    for (int i = 0; i < ceil_log2; ++i)
        res *= (seq_t)5;

    return res;
}
