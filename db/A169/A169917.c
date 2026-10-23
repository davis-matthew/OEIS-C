/*
 * A169917  Squares in carryless arithmetic mod 10 with addition and
 *          multiplication of digits both defined to be multiplication mod 10.
 * https://oeis.org/A169917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n × n under carryless arithmetic modulo 10, where
 *            addition and multiplication of digits are both defined to be
 *            multiplication modulo 10
 * Range:     u8 n <= 19, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* extract decimal digits of n (least‑significant first) */
    uint8_t digits[20];
    int len = 0;
    idx_t tmp = n;
    while (tmp) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* result has at most 2*len digits */
    int reslen = 2 * len;
    seq_t res[40];
    for (int i = 0; i < reslen; ++i)
        res[i] = (seq_t)0;

    /* accumulate the carryless product */
    for (int i = 0; i < len; ++i) {
        uint8_t di = digits[i];
        for (int j = 0; j < len; ++j) {
            int pos = i + j;
            seq_t prod = ((seq_t)di * (seq_t)digits[j]) % (seq_t)10;
            if (res[pos] == (seq_t)0)
                res[pos] = prod;
            else
                res[pos] = (res[pos] * prod) % (seq_t)10;
        }
    }

    /* trim leading zeros */
    int hi = reslen - 1;
    while (hi > 0 && res[hi] == (seq_t)0)
        --hi;

    /* build the final number */
    seq_t result = (seq_t)0;
    for (int i = hi; i >= 0; --i)
        result = result * (seq_t)10 + res[i];

    return result;
}
