/*
 * A212156  a(n) = ((6*A023000(n))^3 + 1)/7^n, n >= 0.
 * https://oeis.org/A212156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^{2n} – 3*7^n + 3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    idx_t e = exp;
    while (e) {
        if (e & 1U)
            result *= cur;
        cur *= cur;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow7   = pow_seq((seq_t)7, n);     /* 7^n            */
    seq_t pow7sq = pow7 * pow7;              /* 7^{2n}         */
    seq_t three  = (seq_t)3;
    return pow7sq - three * pow7 + three;    /* 7^{2n} - 3*7^n + 3 */
}
