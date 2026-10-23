/*
 * A047843  Describe n: give frequency of each digit, by increasing size;
 *          mention also missing digits between the smallest and largest one.
 * https://oeis.org/A047843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 15, u64 n <= 899,
 *            u128 n <= 5000, big any n
 * Helpers:   ndigits_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = numeric value obtained by writing, for every digit d
 * from the smallest present digit of n to the largest present digit of n,
 * the count of d in the decimal representation of n followed by d itself.
 * Missing digits between the smallest and largest are included with a
 * count of 0.
 */
#include <stdint.h>
#include "../utils/ndigits_idx_t.h"

seq_t a(idx_t n) {
    unsigned char cnt[10] = {0};
    idx_t tmp = n;

    /* Count digits of n */
    if (tmp == 0) {
        cnt[0] = 1;
    } else {
        while (tmp) {
            cnt[tmp % 10]++;
            tmp /= 10;
        }
    }

    /* Find smallest and largest present digits */
    int min_d = 0, max_d = 0;
    for (int d = 0; d < 10; d++) if (cnt[d]) { min_d = d; break; }
    for (int d = 9; d >= 0; d--) if (cnt[d]) { max_d = d; break; }

    seq_t res = 0;

    /* Build the result by concatenating count and digit pairs */
    for (int d = min_d; d <= max_d; d++) {
        idx_t c = cnt[d];
        /* Number of decimal digits in count c */
        int len = (c == 0) ? 1 : ndigits_idx_t(c);
        /* pow10 = 10^len */
        seq_t pow10 = (seq_t)1;
        for (int i = 0; i < len; i++) pow10 *= (seq_t)10;

        res = res * pow10 + (seq_t)c;   /* append count */
        res = res * (seq_t)10 + (seq_t)d;/* append digit */
    }

    return res;
}
