/*
 * A095472  a(n) = 40 written in base n.
 * https://oeis.org/A095472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 40 written in base n, concatenated as decimal digits of
 *            the base‑n digits
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    const seq_t VALUE = 40;

    /* Unary base (n == 1) uses 40 consecutive '1' digits. */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < VALUE; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* Extract base‑n digits of VALUE in little‑endian order. */
    seq_t digits[6];      /* 40 in binary needs at most 6 digits. */
    idx_t cnt = 0;

    seq_t temp = VALUE;
    do {
        digits[cnt++] = temp % n;
        temp /= n;
    } while (temp > 0);

    /* Build the concatenated decimal representation, most significant digit first. */
    seq_t result = 0;
    for (idx_t i = cnt; i > 0; --i) {
        seq_t d = digits[i - 1];

        /* Compute decimal length of the digit d (d is unsigned). */
        idx_t len = 1;
        seq_t t = d;
        while (t >= 10) {
            t /= 10;
            ++len;
        }

        result = result * pow10(len) + d;
    }

    return result;
}
