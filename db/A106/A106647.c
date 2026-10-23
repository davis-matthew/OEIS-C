/*
 * A106647  Replace even digits d of n with 1+d/2.
 * https://oeis.org/A106647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = replace each decimal digit d of n with (d odd? d :
 *            1+d/2), then interpret as decimal number. a(0) = 1
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    unsigned digits[20];
    unsigned cnt = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[cnt++] = (unsigned)(tmp % 10);
        tmp /= 10;
    }

    seq_t res = 0;
    for (unsigned i = cnt; i > 0; --i) {
        unsigned d = digits[i - 1];
        unsigned r = (d & 1) ? d : 1 + d / 2;
        res = res * 10 + (seq_t)r;
    }
    return res;
}
