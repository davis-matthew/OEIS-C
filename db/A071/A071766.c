/*
 * A071766  Denominator of the continued fraction expansion whose terms are
 *          the first-order differences of exponents in the binary
 *          representation of 4n, with the exponents of 2 being listed in
 *          descending order.
 * https://oeis.org/A071766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = denominator of the continued fraction whose terms are *
 *            the first‑order differences of the exponents (in descending
 *            order) * in the binary representation of 4n, with an implicit
 *            exponent 0 * appended at the end. * * Implementation uses only
 *            integer arithmetic and works for all n ≥ 0
 * Range:     u8 n <= 6486, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 : a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Compute 4*n as an idx_t value */
    idx_t value = n << 2;            /* 4 * n */

    /* Extract exponents of set bits, in descending order, then append 0 */
    int exp[65];                     /* maximum 64 exponents + extra 0 */
    int exp_cnt = 0;

    for (int i = 63; i >= 0; --i) {
        if ((value >> i) & (idx_t)1)
            exp[exp_cnt++] = i;
    }
    /* Always append exponent 0 */
    exp[exp_cnt++] = 0;

    /* Compute differences between successive exponents */
    int diff[65];
    int diff_cnt = exp_cnt - 1;      /* may be 0 if only one exponent */

    for (int i = 0; i < diff_cnt; ++i)
        diff[i] = exp[i] - exp[i + 1];

    /* Evaluate continued fraction from right to left */
    if (diff_cnt == 0)               /* only a single exponent, denominator is 1 */
        return (seq_t)1;

    seq_t num = (seq_t)diff[diff_cnt - 1];
    seq_t den = (seq_t)1;

    for (int i = diff_cnt - 2; i >= 0; --i) {
        seq_t temp = num;
        num = (seq_t)diff[i] * num + den;
        den = temp;
    }

    return den;
}
