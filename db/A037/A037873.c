/*
 * A037873  (1/2)*Sum{|d(i)-e(i)|}, where Sum{d(i)*5^i} is base 5
 *          representation of n and e(i) are digits d(i) in nonincreasing
 *          order, for i=0,1,...,m.
 * https://oeis.org/A037873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2)*∑|d(i)-e(i)|, where d(i) are the base‑5 digits of n
 *            and e(i) are those digits sorted in non‑increasing order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* collect base‑5 digits of n */
    unsigned char digits[32];
    size_t m = 0;
    idx_t tmp = n;
    do {
        digits[m++] = (unsigned char)(tmp % 5);
        tmp /= 5;
    } while (tmp);

    /* count digits (0..4) */
    unsigned cnt[5] = {0};
    for (size_t i = 0; i < m; ++i)
        ++cnt[digits[i]];

    /* reconstruct sorted digits in non‑increasing order */
    unsigned char sorted[32];
    size_t pos = 0;
    for (int d = 4; d >= 0; --d) {
        unsigned c = cnt[d];
        while (c--)
            sorted[pos++] = (unsigned char)d;
    }

    /* compute sum of absolute differences and halve it */
    seq_t diff_sum = 0;
    for (size_t i = 0; i < m; ++i) {
        seq_t di = (seq_t)digits[i];
        seq_t ei = (seq_t)sorted[i];
        diff_sum += (di > ei) ? (di - ei) : (ei - di);
    }
    return diff_sum / 2;
}
