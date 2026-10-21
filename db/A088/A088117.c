/*
 * A088117  Let the decimal expansion of n be abcd...; then a(n) = (a*bcd... +
 *          b*acd... + c*abd... + d*abc... + ...) + (ab*cd... + bc*ad... +
 *          cd*ab... + ...) + ... . That is, a(n) = sum over all the digit
 *          strings of the product (number obtained by deleting a digit
 *          string) * (deleted digit string).
 * https://oeis.org/A088117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum over all substrings S of length 1..⌊digits(n)/2⌋ of
 *            (number obtained by deleting S) * S, with the restriction that
 *            if n has an even number of digits and a substring has length
 *            exactly n/2, only the first such substring (starting before the
 *            middle) is used
 * Range:     u8 n <= 157, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are defined by the build environment. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Convert n to decimal digits. */
    unsigned char digits[20];
    idx_t len = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = (unsigned char)(tmp % 10);
            tmp /= 10;
        }
        /* Reverse digits to have most significant digit first. */
        for (idx_t i = 0; i < len / 2; ++i) {
            unsigned char swap = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = swap;
        }
    }

    /* Precompute powers of 10. */
    seq_t pow10[21];
    pow10[0] = (seq_t)1;
    for (idx_t i = 1; i <= len; ++i) pow10[i] = pow10[i - 1] * (seq_t)10;

    /* Prefix numbers: prefix[i] = value of first i digits (i from 0 to len). */
    seq_t prefix[21];
    prefix[0] = (seq_t)0;
    for (idx_t i = 0; i < len; ++i)
        prefix[i + 1] = prefix[i] * (seq_t)10 + (seq_t)digits[i];

    /* Suffix numbers: suffix[i] = value of digits from i to len-1. */
    seq_t suffix[22];
    suffix[len] = (seq_t)0;
    for (idx_t i = len; i-- > 0; )
        suffix[i] = (seq_t)digits[i] * pow10[len - i - 1] + suffix[i + 1];

    seq_t total = (seq_t)0;
    idx_t half = len / 2;

    for (idx_t i = 0; i < len; ++i) {
        for (idx_t j = i; j < len; ++j) {
            idx_t L = j - i + 1;
            if (L > half) continue;
            if ((len % 2 == 0) && (L == half) && (i >= half)) continue; /* avoid double counting */
            seq_t sub = prefix[j + 1] - prefix[i] * pow10[L];
            seq_t rem = prefix[i] * pow10[len - j - 1] + suffix[j + 1];
            total += rem * sub;
        }
    }
    return total;
}
