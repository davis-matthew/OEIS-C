/*
 * A064770  Replace each digit of n with the floor of its square root.
 * https://oeis.org/A064770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, nice, look
 * Formula:   a(n) = replace each decimal digit d of n with floor(sqrt(d))
 * Range:     u8 n <= 899, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    unsigned char digits[32];
    int cnt = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[cnt++] = tmp % 10;
        tmp /= 10;
    }
    seq_t res = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        unsigned char d = digits[i];
        unsigned char m;
        if (d == 0) {
            m = 0;
        } else if (d <= 3) {
            m = 1;
        } else if (d <= 8) {
            m = 2;
        } else {
            m = 3;
        }
        res = res * 10 + m;
    }
    return res;
}
