/*
 * A168090  a(n) = (1 - (n mod 3) mod 2)*2^(floor(n/3) + (n mod 3)/2 ).
 * https://oeis.org/A168090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1 - (n mod 3) mod 2) * 2^(floor(n/3) + (n mod 3)/2)
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 94, u64 n <= 190,
 *            u128 n <= 382, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    if ((r % 2) != 0)   /* coefficient is 0 when (n mod 3) is odd */
        return (seq_t)0;
    idx_t exp = (n / 3) + (r >> 1);   /* (n mod 3)/2 == r>>1 */
    return pow2(exp);
}
