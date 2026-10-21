/*
 * A053314  a(n) contains n digits '1' or '4' and is divisible by 2^n.
 * https://oeis.org/A053314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n+1) = a(n) + 10^n * (4 - 3*(a(n)/2^n mod 2)) Initial values:
 *            a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1, return 0 just in case */
    seq_t cur = 4;                   /* a(1) */
    if (n == 1) return cur;

    seq_t pow10 = 10;                /* 10^1, used for computing a(2) */

    for (idx_t i = 1; i < n; ++i) {
        /* determine the new leading digit for a(i+1) */
        uint8_t flag = (uint8_t)((cur >> i) & 1);          /* a(i)/2^i mod 2 */
        uint8_t digit = 4 - 3 * flag;                     /* 4 if flag==0, 1 otherwise */

        cur += pow10 * (seq_t)digit;    /* a(i+1) = a(i) + 10^i * digit */
        pow10 *= 10;                    /* advance 10^i to 10^(i+1) */
    }
    return cur;
}
