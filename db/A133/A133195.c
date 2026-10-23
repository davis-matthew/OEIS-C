/*
 * A133195  Smallest number whose sum of digits is 3n.
 * https://oeis.org/A133195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0; for n>0 let k = n/3 and r=n%3 If r==0: a(n)=10^k - 1, If
 *            r==1: a(n)=4*10^k - 1, If r==2: a(n)=7*10^k - 1
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t k = n / 3;
    idx_t r = n % 3;
    seq_t powk = pow10(k);
    switch (r) {
        case 0: return powk - 1;
        case 1: return powk * 4 - 1;
        case 2: return powk * 7 - 1;
        default: return 0; /* unreachable */
    }
}
