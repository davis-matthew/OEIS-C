/*
 * A137824  Index at which A137823(n) occurs first in A137822 (gaps in numbers
 *          m such that 3 | sum( Catalan(k), k=1..2m)).
 * https://oeis.org/A137824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^[2*(n-1)/3] if n%3 != 2 = 3*2^[2*(n-1)/3] if n%3 == 2
 *            a(1) = 1, a(2)=3, a(3)=2 (offset 1)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 48, u64 n <= 96, u128 n <= 192,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* offset starts at 1 – for n==0 we return 0 */
    if (n == 0) return 0;

    /* e = 2*((n-1)/3)  (integer division) */
    idx_t e = ((n - 1) / 3) * 2;

    /* compute 2^e */
    seq_t power = big_pow((seq_t)2, (seq_t)e);

    /* choose multiplier according to n mod 3 */
    switch (n % 3) {
        case 2:                 /* 3 * 2^e */
            return seq_mul(power, (seq_t)3);
        case 1:                 /* 2^e */
            return power;
        case 0:                 /* 2 * 2^e = 2^(e+1) */
            return seq_mul(power, (seq_t)2);
        default:
            return 0;           /* unreachable */
    }
}
