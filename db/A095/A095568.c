/*
 * A095568  a(n) = 88 written in base n.
 * https://oeis.org/A095568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 88 represented in base n (concatenated in decimal)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: base 1 (unary) - 88 ones */
    if (n == 1) {
        seq_t r = 0;
        for (idx_t i = 0; i < 88; ++i)
            r = r * 10 + 1;
        return r;
    }

    /* 88 written in base n */
    const seq_t val = 88;
    seq_t temp = val;

    /* Store the digits of 88 in base n (least significant first) */
    uint64_t dig[20];
    int dcnt = 0;
    while (temp > 0) {
        seq_t div = temp / (seq_t)n;
        seq_t rem = temp % (seq_t)n;
        dig[dcnt++] = (uint64_t)rem;
        temp = div;
    }
    if (dcnt == 0)  /* shouldn't happen for n > 1 */
        dig[dcnt++] = 0;

    /* Concatenate the digits as decimal numbers */
    seq_t result = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        uint64_t d = dig[i];
        /* Number of decimal digits of d */
        int len = 0;
        uint64_t t = d;
        do {
            len++;
            t /= 10;
        } while (t != 0);

        /* Compute 10^len */
        seq_t pow10 = 1;
        for (int j = 0; j < len; ++j)
            pow10 *= 10;

        result = result * pow10 + (seq_t)d;
    }
    return result;
}
