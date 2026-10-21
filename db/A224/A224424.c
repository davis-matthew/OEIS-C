/*
 * A224424  The hyper-Wiener index of the Micelle-like chiral dendrimer G[n]
 *          defined pictorially in the Hassan Yousefi-Azari et al. reference.
 * https://oeis.org/A224424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2979 + 13680*2^n + 131200*4^n + 52224*n*4^n * +
 *            30240*n*2^n + 36864*n^2*4^n + 6336*n^2*2^n
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t pow2 = 1;   /* 2^0 */
    seq_t pow4 = 1;   /* 4^0 */
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
        pow4 *= (seq_t)4;
    }

    seq_t sn   = (seq_t)n;
    seq_t sn2  = sn * sn;

    seq_t result = 2979;
    result += 13680 * pow2;
    result += 131200 * pow4;
    result += 52224  * sn   * pow4;
    result += 30240  * sn   * pow2;
    result += 36864  * sn2  * pow4;
    result += 6336   * sn2  * pow2;
    return result;
}
