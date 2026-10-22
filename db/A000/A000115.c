/*
 * A000115  Denumerants: Expansion of 1/((1-x)*(1-x^2)*(1-x^5)).
 * https://oeis.org/A000115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((n+4)^2/20) = floor(((n+4)^2 + 10)/20)
 * Range:     u8 n <= 67, u16 n <= 1140, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Split m = n+4 as 20q + r so that intermediates stay within the result:
       floor((m^2+10)/20) = 20q^2 + 2qr + floor((r^2+10)/20). */
    idx_t m = n + 4;
    idx_t q = m / 20;
    unsigned r = (unsigned)(m % 20);
    seq_t Q = (seq_t)q;
    seq_t res = (seq_t)20 * Q * Q;
    res += (seq_t)(2 * r) * Q;
    res += (seq_t)((r * r + 10) / 20);
    return res;
}
