/*
 * A122635  a(n) = {n^2}_n.
 * https://oeis.org/A122635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret the decimal digits of n² as digits in base n
 * Range:     u8 n <= 12, u16 n <= 38, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* n² */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* Extract decimal digits of n² (least‑significant first) */
    unsigned char digit[80];
    size_t dcnt = 0;
    seq_t tmp = sq;
    if (tmp == 0) {
        digit[0] = 0;
        dcnt = 1;
    } else {
        while (tmp) {
            digit[dcnt++] = (unsigned char)(tmp % 10);
            tmp /= 10;
        }
    }

    /* Interpret those digits as a base‑n number */
    seq_t res = 0;
    for (long long i = (long long)dcnt - 1; i >= 0; --i) {
        res = res * (seq_t)n + (seq_t)digit[i];
    }
    return res;
}
