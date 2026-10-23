/*
 * A117909  Count, inserting 0 after every even number.
 * https://oeis.org/A117909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n mod 3 = 2; otherwise a(n) = 2*floor(n/3) + (n mod
 *            3==0 ? 1 : 2)
 * Range:     u8 n <= 381, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    idx_t k = n / 3;
    idx_t r = n % 3;
    if (r == 2) return 0;
    seq_t kk = (seq_t)k;
    return kk * 2 + ((r == 0) ? 1 : 2);
}
