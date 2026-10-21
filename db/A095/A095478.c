/*
 * A095478  a(n) = 43 written in base n.
 * https://oeis.org/A095478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal number obtained by writing the integer 43 in base
 *            n and interpreting the resulting base‑n digits as a decimal
 *            string
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Unary base (base 1) – 43 is written as 43 ones */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 43; ++i)
            res = res * 10 + 1;
        return res;
    }

    const idx_t target = 43;
    idx_t val = target;
    /* digits least significant first; at most 6 digits for 43 in any base >1 */
    idx_t digits[16];
    idx_t cnt = 0;
    while (val > 0) {
        digits[cnt++] = val % n;
        val /= n;
    }

    seq_t res = 0;
    /* Append digits from most significant to least, converting each base‑n
       digit to its decimal representation. */
    for (idx_t i = cnt; i > 0; --i) {
        idx_t d = digits[i - 1];

        /* count decimal digits of d */
        idx_t k = 1;
        idx_t t = d;
        while (t >= 10) {
            t /= 10;
            ++k;
        }

        seq_t pow = pow10(k);
        res = res * pow + (seq_t)d;
    }
    return res;
}
