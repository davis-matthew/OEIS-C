/*
 * A160018  A175099 with a(3) changed from 0 to 1.
 * https://oeis.org/A160018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=0; for k≥1, a(2^k)=2^(k-1)-1; for 1≤i<2^k,
 *            a(2^k+i)=2*a(i); note a(3) is overridden to 1
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* find largest power of two ≤ n */
    idx_t power = 1;
    while (power <= n / 2) {
        power <<= 1;
    }

    if (n == power) {
        /* compute k = log₂(power) */
        idx_t k = 0;
        idx_t tmp = power;
        while (tmp > 1) {
            tmp >>= 1;
            ++k;
        }
        /* compute 2^(k-1) - 1 */
        seq_t val = 1;
        for (idx_t i = 0; i < k - 1; ++i) {
            val = val * 2;
        }
        val = val - 1;
        return val;
    }

    idx_t i = n - power;
    if (n == 3) return 1;        /* override a(3) */
    seq_t ai = a(i);            /* recursive call */
    return ai + ai;             /* 2 * a(i) */
}
