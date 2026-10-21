/*
 * A051441  4^3^2^1, 5^4^3, 6^5, 7.
 * https://oeis.org/A051441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = (n+4)^( (n+3)^( (n+2)^( ... 1 ) ) ) with the exponent
 *            tower truncated so that its height is max(0,3-n). Thus
 *            a(0)=4^(3^2^1), a(1)=5^(4^3), a(2)=6^5, a(n)=n+4 for n≥3
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 0, u128 n <= 0,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)(n + 4);
    if (n >= 3)
        return base;

    if (n == 0) {
        seq_t e = big_pow((seq_t)2, (seq_t)1);   /* 2^1 = 2          */
        e = big_pow((seq_t)3, e);               /* 3^2 = 9          */
        return big_pow(base, e);                /* 4^9 = 262144     */
    }

    if (n == 1) {
        seq_t e = big_pow((seq_t)4, (seq_t)3);   /* 4^3 = 64         */
        return big_pow(base, e);                /* 5^64             */
    }

    /* n == 2 */
    return big_pow(base, (seq_t)5);              /* 6^5 = 7776       */
}
