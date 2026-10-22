/*
 * A002239  9th powers written backwards.
 * https://oeis.org/A002239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = digit reversal of n^9 (base 10); n^9 is formed exactly as
 *            a decimal digit array, then its digits are read back in reverse
 *            into seq_t
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 12, u64 n <= 131,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

seq_t a(idx_t n) {
    int nd[24], ndl = 0;
    int cur[256], tmp[256];
    int cl, i, j, k;
    seq_t r = 0;
    idx_t m = n;

    if (n == 0) return (seq_t)0;
    while (m > 0) { nd[ndl++] = (int)(m % 10); m /= 10; }

    memset(cur, 0, sizeof(cur));
    for (i = 0; i < ndl; i++) cur[i] = nd[i];
    cl = ndl;

    for (k = 1; k < 9; k++) {
        long long acc[256];
        int tl = cl + ndl;
        long long carry = 0;
        for (i = 0; i < tl + 1; i++) acc[i] = 0;
        for (i = 0; i < cl; i++)
            for (j = 0; j < ndl; j++)
                acc[i + j] += (long long)cur[i] * nd[j];
        for (i = 0; i < tl; i++) {
            long long v = acc[i] + carry;
            tmp[i] = (int)(v % 10);
            carry = v / 10;
        }
        while (carry > 0) { tmp[tl++] = (int)(carry % 10); carry /= 10; }
        while (tl > 1 && tmp[tl - 1] == 0) tl--;
        for (i = 0; i < tl; i++) cur[i] = tmp[i];
        cl = tl;
    }

    for (i = 0; i < cl; i++) r = r * 10 + (seq_t)cur[i];
    return r;
}
