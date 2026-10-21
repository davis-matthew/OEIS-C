/*
 * A095528  a(n) = 68 written in base n.
 * https://oeis.org/A095528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal representation of 68 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"      /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h" /* int digit_len(seq_t x); */

seq_t a(idx_t n)
{
    const seq_t N = 68;

    /* Base 1 uses 68 ones */
    if (n == 1) {
        seq_t v = pow10(68);   /* 10^68 */
        v -= 1;                /* 10^68 - 1 */
        v = v / 9;             /* (10^68 - 1)/9 = 111...1 (68 digits) */
        return v;
    }

    /* Compute digits of N in base n */
    seq_t temp = N;
    seq_t digits[64];
    int sz = 0;
    while (temp) {
        digits[sz++] = temp % n;
        temp = temp / n;
    }

    /* Concatenate digits as decimal string in a seq_t */
    seq_t res = 0;
    for (int i = sz - 1; i >= 0; --i) {
        seq_t d = digits[i];
        int k = digit_len(d);          /* number of decimal digits of d */
        seq_t factor = pow10(k);       /* 10^k */
        res = res * factor + d;
    }
    return res;
}
