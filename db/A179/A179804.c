/*
 * A179804  Number of letter combinations on a standard telephone keypad
 *          represented by the digits in n.
 * https://oeis.org/A179804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 0 if n contains any digit 0 or 1; * otherwise a(n) = 3^d
 *            where d is the number of decimal digits of n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* provides big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    int d = 0;

    /* Scan digits from least significant to most significant. */
    while (tmp > 0) {
        unsigned dig = (unsigned)(tmp % 10);
        if (dig == 0 || dig == 1)
            return 0;               /* forbidden digit found */
        ++d;
        tmp /= 10;
    }

    /* All digits are in {2..9}.  Compute 3^d. */
    return big_pow(3, (seq_t)d);
}
