/*
 * A087052  Lunar triangular numbers: 0+1+2+3+...+n, where + is lunar
 *          addition.
 * https://oeis.org/A087052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i≥0} min(9, ⌊n / 10^i⌋) * 10^i (maximum digit at each
 *            decimal position over 0 … n)
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t pow10 = 1;                     /* 10^i */
    while (pow10 <= n) {
        idx_t max_digit = n / pow10;     /* digit seen at this place */
        if (max_digit > 9) max_digit = 9;/* limit to single decimal digit    */
        result += (seq_t)max_digit * (seq_t)pow10;
        /* avoid overflow: if next pow10 would overflow or exceed n, stop */
        if (pow10 > n / 10) break;
        pow10 *= 10;
    }
    return result;
}
