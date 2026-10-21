/*
 * A095558  a(n) = 83 written in base n.
 * https://oeis.org/A095558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = representation of the decimal number 83 written in base n
 *            (digits of the base‑n representation concatenated as decimal
 *            digits)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* unary base (base 1) */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 83; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* digits of 83 in base n (least–significant first) */
    idx_t temp = 83;
    idx_t digits[64];     /* 83 in base 2 needs at most 7 digits, so 64 is safe */
    int cnt = 0;
    while (temp > 0) {
        digits[cnt++] = temp % n;
        temp /= n;
    }

    /* build the decimal concatenation in most–significant order */
    seq_t res = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        idx_t d = digits[i];

        /* number of decimal digits of d */
        idx_t k = 0;
        idx_t t = d;
        do {
            ++k;
            t /= 10;
        } while (t != 0);

        /* pow = 10^k */
        seq_t pow = 1;
        for (idx_t j = 0; j < k; ++j)
            pow *= 10;

        res = res * pow + (seq_t)d;
    }

    return res;
}
